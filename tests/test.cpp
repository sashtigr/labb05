#include <gtest/gtest.h>

#include <Transaction.h>
#include <Account.h>

TEST(Transaction, fee_test) {
    Account John(228, 101);
    Account James(1337, 0);

    Transaction transaction;
    EXPECT_TRUE(transaction.Make(John, James, 100));

    Account Jack(111, 100);
    EXPECT_FALSE(transaction.Make(Jack, James, 100));

    Jack.Lock();
    Jack.ChangeBalance(-10);
    Jack.Unlock();
    EXPECT_FALSE(transaction.Make(Jack, James, 100));
}

TEST(Transaction, id_is_equal) {
    Account John(228, 1);
    Account Peter(228, 1);

    Transaction transaction;
    try {
        transaction.Make(John, Peter, 100);
        FAIL() << "expected std::logic_error";
    } catch (std::logic_error& err){
        EXPECT_EQ(err.what(), std::string("invalid action"));
    } catch(...) {
        FAIL() << "expected std::logic_error";
    }
}

TEST(Transaction, negative_sum) {
    Account John(228, 123);
    Account Peter(123, 0);

    Transaction transaction;
    try {
        transaction.Make(John, Peter, -12);
        FAIL() << "expected std::invalid argument";
    } catch(std::invalid_argument& err) {
        EXPECT_EQ(err.what(), std::string("sum can't be negative"));
    } catch(...) {
        FAIL() << "expected std::invalid argument";
    }
}

TEST(Transaction, low_sum) {
    Account John(228, 1200);
    Account Peter(123, 0);

    Transaction transaction;
    try {
        transaction.Make(John, Peter, 99);
        FAIL() << "expected std::logic_error";
    } catch(std::logic_error& err) {
        EXPECT_EQ(err.what(), std::string("too small"));
    } catch(...) {
        FAIL() << "expected std::logic error";
    }
}

TEST(Transaction, too_big_fee) {
    Account John(128, 400);
    Account Jake(200, 0);

    Transaction transaction;
    transaction.set_fee(101);

    EXPECT_FALSE(transaction.Make(John, Jake, 200));
}

TEST(Account, account_tests) {
    Account account_1(100, 1000);

    EXPECT_EQ(1000, account_1.GetBalance());

    account_1.Lock();
    account_1.ChangeBalance(100);
    EXPECT_ANY_THROW(account_1.Lock());
    EXPECT_EQ(account_1.GetBalance(), 1100);

    account_1.Unlock();
    EXPECT_ANY_THROW(account_1.ChangeBalance(100));
}
