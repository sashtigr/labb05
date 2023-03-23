## Laboratory work V

Данная лабораторная работа посвещена изучению фреймворков для тестирования на примере **GTest**

```sh
$ open https://github.com/google/googletest
```

## Tasks

- [ ] 1. Создать публичный репозиторий с названием **lab05** на сервисе **GitHub**
- [ ] 2. Выполнить инструкцию учебного материала
- [ ] 3. Ознакомиться со ссылками учебного материала
- [ ] 4. Составить отчет и отправить ссылку личным сообщением в **Slack**

## Tutorial

```sh
$ export GITHUB_USERNAME=<имя_пользователя>
$ alias gsed=sed # for *-nix system
```

```sh
$ cd ${GITHUB_USERNAME}/workspace
$ pushd .
$ source scripts/activate
```

```sh
$ git clone https://github.com/${GITHUB_USERNAME}/lab04 projects/lab05
$ cd projects/lab05
$ git remote remove origin
$ git remote add origin https://github.com/${GITHUB_USERNAME}/lab05
```

```sh
$ mkdir third-party
$ git submodule add https://github.com/google/googletest third-party/gtest
$ cd third-party/gtest && git checkout release-1.8.1 && cd ../..
$ git add third-party/gtest
$ git commit -m"added gtest framework"
```

```sh
$ gsed -i '/option(BUILD_EXAMPLES "Build examples" OFF)/a\
option(BUILD_TESTS "Build tests" OFF)
' CMakeLists.txt
$ cat >> CMakeLists.txt <<EOF

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  file(GLOB \${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check \${PROJECT_NAME} gtest_main)
  add_test(NAME check COMMAND check)
endif()
EOF
```

```sh
$ mkdir tests
$ cat > tests/test1.cpp <<EOF
#include <print.hpp>

#include <gtest/gtest.h>

TEST(Print, InFileStream)
{
  std::string filepath = "file.txt";
  std::string text = "hello";
  std::ofstream out{filepath};

  print(text, out);
  out.close();

  std::string result;
  std::ifstream in{filepath};
  in >> result;

  EXPECT_EQ(result, text);
}
EOF
```

```sh
$ cmake -H. -B_build -DBUILD_TESTS=ON
$ cmake --build _build
$ cmake --build _build --target test
```

```sh
$ _build/check
$ cmake --build _build --target test -- ARGS=--verbose
```

```sh
$ gsed -i 's/lab04/lab05/g' README.md
$ gsed -i 's/\(DCMAKE_INSTALL_PREFIX=_install\)/\1 -DBUILD_TESTS=ON/' .travis.yml
$ gsed -i '/cmake --build _build --target install/a\
- cmake --build _build --target test -- ARGS=--verbose
' .travis.yml
```

```sh
$ travis lint
```

```sh
$ git add .travis.yml
$ git add tests
$ git add -p
$ git commit -m"added tests"
$ git push origin master
```

```sh
$ travis login --auto
$ travis enable
```

```sh
$ mkdir artifacts
$ sleep 20s && gnome-screenshot --file artifacts/screenshot.png
# for macOS: $ screencapture -T 20 artifacts/screenshot.png
# open https://github.com/${GITHUB_USERNAME}/lab05
```

## Report

```sh
$ popd
$ export LAB_NUMBER=05
$ git clone https://github.com/tp-labs/lab${LAB_NUMBER} tasks/lab${LAB_NUMBER}
$ mkdir reports/lab${LAB_NUMBER}
$ cp tasks/lab${LAB_NUMBER}/README.md reports/lab${LAB_NUMBER}/REPORT.md
$ cd reports/lab${LAB_NUMBER}
$ edit REPORT.md
$ gist REPORT.md
```

## Homework

### Задание
1. Создайте `CMakeList.txt` для библиотеки *banking*.
2. Создайте модульные тесты на классы `Transaction` и `Account`.
    * Используйте mock-объекты.
    * Покрытие кода должно составлять 100%.
3. Настройте сборочную процедуру на **TravisCI**.
4. Настройте [Coveralls.io](https://coveralls.io/).



1. В banking в CMakeLists пишем:

cmake_minimum_required(VERSION 3.4)

project(banking)

add_library(banking STATIC Account.cpp Transaction.cpp)

2. Создадим через mkdit папку с названием test.
В ней соответсвенно 3 файла: test-account.cpp
test-Transaction.cpp test1.cpp

3. ## В test-account пишем
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



## В test-Transaction пишем

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




##В test1 пишем

#include <gtest/gtest.h>
#include "gmock/gmock.h"
#include <Transaction.h>
#include <Account.h>


class MockAccount: public Account{
public:
    //MockAccount(int id, int balance):Account(id, balance){}
    //MOCK_METHOD(void, Unlock, ());
    MockAccount(int id, int balance):Account(id, balance){}

    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
};
TEST(Account, Init){
    MockAccount test(1,100);
    EXPECT_EQ(test.GetBalance(),100);
    EXPECT_EQ(test.id(),1);
}

class MockTransaction: public Transaction{
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(Account, GetBalance){
MockAccount acc(1,100);
EXPECT_EQ(acc.Account::GetBalance(), 100);
}

TEST(Account, ChangeBalance){
MockAccount acc(0, 100);
EXPECT_THROW(acc.Account::ChangeBalance(50), std::runtime_error);
acc.Account::Lock();
acc.Account::ChangeBalance(50);
EXPECT_EQ(acc.Account::GetBalance(), 150);

}

TEST(Account, Lock) {
    MockAccount acc(15,213);
    acc.Lock();
    EXPECT_THROW(acc.Lock(), std::runtime_error);
}

TEST(Account, Unlock){
    MockAccount acc(0, 100);
EXPECT_CALL(acc, Unlock()).Times(1);
acc.Unlock();
}

TEST(Transaction, Make){
MockAccount from_acc(15,17650);
MockAccount to_acc(13, 18435);
MockTransaction tr;
EXPECT_THROW(tr.Make(from_acc,from_acc,0),std::logic_error);
EXPECT_THROW(tr.Make(from_acc,to_acc,0),std::logic_error);
EXPECT_THROW(tr.Make(from_acc,to_acc,-5),std::invalid_argument);
}

TEST(Transaction, SaveToDataBase){
MockAccount from_acc(15,17650);
MockAccount to_acc(13, 18435);
MockTransaction tr;


}



##4. Cоздадим папку third-party
откроем ее через теминал и напишем

git submodule add git@github.com:google/googletest.git

git submodule add git@github.com:JoakimSoderberg/coveralls-cmake.git

git add git commit git push


5, В первос пункте создали CMakeLists.
Через терминал заходим в banking

cmake . -B _build        cd _build     make


6.

## Links

- [C++ CI: Travis, CMake, GTest, Coveralls & Appveyor](http://david-grs.github.io/cpp-clang-travis-cmake-gtest-coveralls-appveyor/)
- [Boost.Tests](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/)
- [Catch](https://github.com/catchorg/Catch2)

```
Copyright (c) 2015-2021 The ISC Authors
```
