#include "sbase.h"
#include <fstream>


SBase::SBase() : connection_(nullptr)
{

}

bool SBase::Initialise(const std::string &db_filepath)
{
    std::ifstream check_file(db_filepath.c_str());
    if (check_file.good()) {
        database_location_ = db_filepath;
    } else {
        std::cerr << "ERROR: Could not find file: " << db_filepath << std::endl;
        return false;
    }
    if (!OpenDatabase()) {
        std::cerr << "ERROR: Could not open config database" << std::endl;
        return false;
    }
    return true;
}

bool SBase::OpenDatabase(void)
{
    int ret = sqlite3_open(database_location_.c_str(), &connection_);
    if (ret != SQLITE_OK) {
        std::cerr << "Unable to open database: " << sqlite3_errmsg(connection_) << std::endl;
        sqlite3_close(connection_);
        return false;
    }
    return true;
}

bool SBase::CreateTable(void)
{
    if (connection_ == nullptr) {
        std::cerr << "ERROR:connection closed" << std::endl;
        return false;
    }
    char* errMsg = nullptr;
    std::string CreateTableQuery = "CREATE TABLE IF NOT EXISTS user_table (id INTEGER PRIMARY KEY, passw TEXT, name TEXT, icon TEXT)";
    if (!ExecuteSqlQuery(CreateTableQuery)) {
        return false;
    }
    return true;
}

bool SBase::Insert(int id, std::string passw, std::string name, std::string icon)
{
    std::string insert_query =
        "INSERT INTO user_table"
        "(id,passw,name,icon)"
        " VALUES(" + std::to_string(id) + ", \'" + passw + "\', \'" +
        name + "\', \'" + icon + "\')";
    if (!ExecuteSqlQuery(insert_query)) {
        return false;
    }
    return true;
}

bool SBase::ReadByID(int id, const std::string read, std::string *return_value)
{
    std::string sql_query;
    sql_query = "SELECT " + read + " FROM user_table WHERE " + \
                "id=" + std::to_string(id);

    std::vector<std::string> query_results;
    if (!ExecuteSqlQuery(sql_query, &query_results)) {
        return false;
    }
    if (query_results.size() == 0) {
        return false;
    }

    *return_value = query_results.front();
    return true;
}

bool SBase::UpdateByID(int id, const std::string update, const std::string value)
{
    std::string sql_query = "UPDATE user_table SET " + update + "=\'" + value +
                            "\' WHERE id=" + std::to_string(id);
    if (!ExecuteSqlQuery(sql_query)) {
        return false;
    }
    return true;
}

int SBase::Count(void)
{
    std::vector<std::string> query_results;
    std::string sql_query = "SELECT COUNT(*) FROM user_table";
    if (!ExecuteSqlQuery(sql_query, &query_results)) {
        return 1000;
    }
    return atoi((query_results.front()).c_str());
}

std::string SBase::GenerateInviteCode(void)
{
    std::vector<std::string> query_results;
    std::string sql_query = "SELECT printf('%06d', ABS(RANDOM()) % 1000000) as random_number";
    ExecuteSqlQuery(sql_query, &query_results);
    return query_results.front();
}

std::string SBase::GetInviteCode(void)
{
    std::vector<std::string> query_results;
    std::string sql_query = "SELECT code FROM invite_code_table WHERE id=1";
    ExecuteSqlQuery(sql_query, &query_results);
    return query_results.front();
}

bool SBase::ChangeInviteCode(std::string new_code)
{
    std::string sql_query = "UPDATE invite_code_table SET code=\'" + new_code +
                            "\' WHERE id=1";
    if (!ExecuteSqlQuery(sql_query)) {
        return false;
    }
    sql_query = "UPDATE invite_code_table SET times=0 WHERE id=1";
    if (!ExecuteSqlQuery(sql_query)) {
        return false;
    }
    return true;
}

int SBase::GetInviteCodeTimes(void)
{
    std::vector<std::string> query_results;
    std::string sql_query = "SELECT times FROM invite_code_table WHERE id=1";
    ExecuteSqlQuery(sql_query, &query_results);
    return atoi(query_results.front().c_str());
}

bool SBase::UpdateInviteCodeTimes(int times)
{
    std::string sql_query = "UPDATE invite_code_table SET times=" + std::to_string(times) + " WHERE id=1";
    if (!ExecuteSqlQuery(sql_query)) {
        return false;
    }
    return true;
}

bool SBase::ExecuteSqlQuery(const std::string &sqlquery, std::vector<std::string> *return_rows)
{
    if (connection_ == nullptr) {
        std::cerr << "ERROR: connection closed" << std::endl;
        return false;
    }
    sqlite3_stmt *query;

    if (sqlite3_prepare_v2(connection_, sqlquery.c_str(), -1, &query, nullptr) != SQLITE_OK) {
        std::cerr << "ERROR (sqlite): " << sqlite3_errmsg(connection_);
        return false;
    }
    int step_result;

    while (SQLITE_ROW == (step_result = sqlite3_step(query))) {
        std::string row_string;
        int column_count = sqlite3_column_count(query);
        for (int i = 0; i < column_count; i++) {
            if (sqlite3_column_text(query, i) != nullptr) {
                if (i != 0) {
                    row_string += "|";
                }
                row_string += std::string(reinterpret_cast<const char *>(sqlite3_column_text(query, i)));
            }
        }
        return_rows->push_back(row_string);
    }

    bool ret = true;
    if (step_result != SQLITE_DONE) {
        std::cerr << "ERROR: " << step_result << std::endl;
        ret = false;
    }

    if (sqlite3_finalize(query) != SQLITE_OK) {
        std::cerr << "ERROR: SQLITE3 Statement not destructed" << std::endl;
        ret = false;
    }
    return ret;
}
