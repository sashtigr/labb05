#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include <Transaction.h>
#include <Account.h>


class MockAccount: public Account{
public:
    MockAccount(int id, int balance):Account(id, balance){}

    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction: public Transaction{
private:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(Transaction, Make)
{
    MockAccount from_ac(1,1000);
    MockAccount to_ac(2, 2000);
    MockTransaction tr;

    EXPECT_THROW(tr.Make(from_ac,from_ac, 0), std::logic_error);
    EXPECT_THROW(tr.Make(from_ac,to_ac, 0), std::invalid_argument);
    EXPECT_THROW(tr.Make(from_ac,to_ac,-5), std::invalid_argument);
    EXPECT_THROW(tr.Make(from_ac, to_ac, 50), std::logic_error);
}
