#include <iostream>
#include "sbase.h"

int main(void)
{
    SBase* sbase = new SBase();

    sbase->Initialise("./wefish.db");

    sbase->Insert(522000, "xxxxxx", "root", "xxxxxx");

    std::string ret;
    sbase->ReadByID(522000, "name", &ret);
    std::cout << ret << std::endl;
    sbase->UpdateByID(522000, "name", "Anita");
    sbase->ReadByID(522000, "name", &ret);
    std::cout << ret << std::endl;
    int count = 0;
    count = sbase->Count();
    std::cout << "User Num: " << count << std::endl;

    ret = sbase->GetInviteCode();
    std::cout << "Origin Invite Code: " << ret << std::endl;
    ret = sbase->GenerateInviteCode();
    std::cout << "New Invite Code: " << ret << std::endl;
    sbase->ChangeInviteCode(ret);
    ret = sbase->GetInviteCode();
    count = sbase->GetInviteCodeTimes();
    std::cout << "Origin Invite code: " << ret << ", Times: " << std::to_string(count) << std::endl;
    sbase->UpdateInviteCodeTimes(count + 1);
    count = sbase->GetInviteCodeTimes();
    std::cout << "Origin Invite code: " << ret << ", Times: " << std::to_string(count) << std::endl;
}