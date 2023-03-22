#include <gtest/gtest.h>
#include "gmock/gmock.h"

#include <Account.h>


class MockAccount: public Account{
public:
    MockAccount(int id, int balance):Account(id, balance){}

    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};


TEST(Account, Init)
{
    MockAccount ac(1, 150);
    EXPECT_EQ(ac.GetBalance(), 150);
    EXPECT_EQ(ac.id(), 1);
}

TEST(Account, GetBalance)
{
    MockAccount ac(1,150);
    EXPECT_EQ(ac.GetBalance(), 150);
}

TEST(Account, ChangeBalance)
{
    MockAccount ac(1, 150);
    EXPECT_THROW(ac.ChangeBalance(50), std::runtime_error);
    ac.Lock();
    ac.ChangeBalance(50);
    EXPECT_EQ(ac.GetBalance(), 200);
}

TEST(Account, Lock)
{
    MockAccount ac(1, 150);
    ac.Lock();
    EXPECT_THROW(ac.Lock(), std::runtime_error);
}

TEST(Account, Unlock)
{
    MockAccount ac(1, 150);
    ac.Lock();
    ac.Unlock();
    EXPECT_THROW(ac.ChangeBalance(50), std::runtime_error);
}
