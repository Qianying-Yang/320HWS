#include <criterion/criterion.h>
#include <criterion/logging.h>
#include "hw1.h"

Test(hw1_tests_suite, help_test01) {
    int argc = 2;
    char *argv[] = {"bin/hw1", "-h", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test02) {
    int argc = 3;
    char *argv[] = {"bin/hw1", "-h", "-p", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test03) {
    int argc = 3;
    char *argv[] = {"bin/hw1", "-h", "-f", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test04) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-h", "-f", "-d", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test05) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-h", "-f", "-e", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test06) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test07) {
    int argc = 4;
    char *argv[] = {"bin/hw1", "-h", "-p", "-e", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test08) {
    int argc = 5;
    char *argv[] = {"bin/hw1", "-h", "-f", "-d", "-k", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test09) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-f", "-d", "-k", "FKUJWONG",NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test10) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-f", "-e", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test11) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-e", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test12) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test13) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-k", "FKUJWONG", "-r", "5", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test14) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-k", "FKUJWONG", "-c", "5", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test15) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-k", "FKUJWONG", "-c", "5", "-r", "5", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test16) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-k", "FKUJWONG", "-r", "5", "-c", "5", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test17) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-r", "5", "-k", "FKUJWONG", "-c", "5", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test18) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-c", "5", "-k", "FKUJWONG", "-r", "5", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test19) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-c", "5", "-r", "5", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test20) {
    int argc = 6;
    char *argv[] = {"bin/hw1", "-h", "-p", "-d", "-r", "5", "-c", "5", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x8000;

    cr_assert((ret&exp) == exp, "Did not sign -h properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test21) {
    int argc = 1;
    char *argv[] = {"bin/hw1", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x0000;

    cr_assert((ret&exp) == exp, "Did not sign error properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test22) {
    int argc = 2;
    char *argv[] = {"bin/hw1", "-p", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x0000;

    cr_assert((ret&exp) == exp, "Did not sign error properly. Expected: %d | Got: %d", exp, ret);
}


Test(hw1_tests_suite, help_test23) {
    int argc = 2;
    char *argv[] = {"bin/hw1", "-f", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x0000;

    cr_assert((ret&exp) == exp, "Did not sign error properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test24) {
    int argc = 3;
    char *argv[] = {"bin/hw1", "-f", "-d", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x6000;

    cr_assert((ret&exp) == exp, "Did not sign -f -d properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test25) {
    int argc = 3;
    char *argv[] = {"bin/hw1", "-f", "-e", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x4000;

    cr_assert((ret&exp) == exp, "Did not sign -f -e properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test26) {
    int argc = 3;
    char *argv[] = {"bin/hw1", "-p", "-d", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20AA;

    cr_assert((ret&exp) == exp, "Did not sign -p -d properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test27) {
    int argc = 3;
    char *argv[] = {"bin/hw1", "-p", "-e", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00AA;

    cr_assert((ret&exp) == exp, "Did not sign -p -e properly. Expected: %d | Got: %d", exp, ret);
}

// Test(hw1_tests_suite, help_test28) {
//     int argc = 4;
//     char *argv[] = {"bin/hw1", "-f", "-d", "-k", NULL};

//     unsigned short ret = validargs(argc, argv);
//     unsigned short exp = 0x0000;

//     cr_assert((ret&exp) == exp, "Did not sign error properly. Expected: %d | Got: %d", exp, ret);
// }

Test(hw1_tests_suite, help_test29) {
    int argc = 5;
    char *argv[] = {"bin/hw1", "-f", "-d", "-k", "FKUJWONG",NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x6000;

    cr_assert((ret&exp) == exp, "Did not sign -f -d -k properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test30) {
    int argc = 5;
    char *argv[] = {"bin/hw1", "-f", "-e", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x4000;

    cr_assert((ret&exp) == exp, "Did not sign -f -e -k properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test31) {
    int argc = 5;
    char *argv[] = {"bin/hw1", "-p", "-e", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00AA;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test32) {
    int argc = 5;
    char *argv[] = {"bin/hw1", "-p", "-d", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20AA;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test33) {
    int argc = 7;
    char *argv[] = {"bin/hw1", "-p", "-d", "-k", "FKUJWONG", "-r", "13", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CA;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test34) {
    int argc = 7;
    char *argv[] = {"bin/hw1", "-p", "-d", "-k", "FKUJWONG", "-c", "13", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20AC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test35) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-d", "-k", "FKUJWONG", "-c", "12", "-r", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test36) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-d", "-k", "FKUJWONG", "-r", "12", "-c", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test37) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-d", "-r", "12", "-k", "FKUJWONG", "-c", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test38) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-d", "-c", "12", "-k", "FKUJWONG", "-r", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test39) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-d", "-c", "12", "-r", "12", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test40) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-d", "-r", "12", "-c", "12", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x20CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -d -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test41) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-e", "-k", "FKUJWONG", "-c", "12", "-r", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test42) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-e", "-k", "FKUJWONG", "-r", "12", "-c", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test43) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-e", "-r", "12", "-k", "FKUJWONG", "-c", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    printf("%d\n", ret);
    unsigned short exp = 0x00CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test44) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-e", "-c", "12", "-k", "FKUJWONG", "-r", "12", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test45) {
    int argc = 9;
    char *argv[] = {"bin/hw1","-p", "-e", "-c", "12", "-r", "12", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}

Test(hw1_tests_suite, help_test46) {
    int argc = 9;
    char *argv[] = {"bin/hw1", "-p", "-e", "-r", "12", "-c", "12", "-k", "FKUJWONG", NULL};

    unsigned short ret = validargs(argc, argv);
    unsigned short exp = 0x00CC;

    cr_assert((ret&exp) == exp, "Did not sign -p -e -k -c -r properly. Expected: %d | Got: %d", exp, ret);
}


// Test(hw1_tests_suite, test02) {
//     int argc = 5;
//     char *argv[] = {"bin/hw1", "-f", "-e", "-k", "ARGS", NULL};

//     unsigned short ret = validargs(argc, argv);
//     unsigned short exp = 0x4000;

//     cr_assert_eq(ret & exp, exp, "Invalid value of valid args. Got: %d | Expected: %d"
//         , ret, exp);
// }

// Test(hw1_tests_suite, test03) {
//     char *cmd = "bin/hw1 -h";

//     // system is a syscall defined in stdlib.h
//     // it takes a shell command as a string and runs it
//     // we use WEXITSTATUS to get the return code from the run
//     // use 'man 3 system' to find out more
//     int return_code = WEXITSTATUS(system(cmd));

//     cr_assert_eq(return_code, EXIT_SUCCESS, "Program exited with %d instead of EXIT_SUCESS", return_code);
// }

