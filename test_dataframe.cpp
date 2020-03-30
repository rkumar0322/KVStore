
#include "dataframe.h"
#include "row.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>

#define GT_TRUE(a)   ASSERT_EQ((a),true)
#define GT_FALSE(a)  ASSERT_EQ((a),false)
#define GT_EQUALS(a, b)   ASSERT_EQ(a, b)
#define ASSERT_EXIT_ZERO(a)  \
  ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*")

void testmap_Fib() {
  Schema s("IIIIIIIIII");

  DataFrame df(s);
  Row  r(df.get_schema());
  for(int i = 0; i < 1200 * 1000; i++) {
    r.set(0, i);
    r.set(1, (i+1));
    df.add_row(r);
  }

  exit(0);
}

void testpmap_Fib() {
  Schema s("IIIIIIIIII");

  DataFrame df(s);
  Row  r(df.get_schema());
  for(int i = 0; i < 1200 * 1000; i++) {
    r.set(0, i);
    r.set(1, (i+1));
    df.add_row(r);
  }

  exit(0);
}

void testmap_RCG() {
  Schema s("SSSSSSSSSS");

  DataFrame df(s);
  Row  r(df.get_schema());
  for(int i = 0; i < 600 * 1000; i++) {
    df.add_row(r);
  }

  exit(0);
}

void testpmap_RCG() {
  Schema s("SSSSSSSSSS");

  DataFrame df(s);
  Row  r(df.get_schema());
  for(int i = 0; i < 600 * 1000; i++) {
    df.add_row(r);
  }

  exit(0);
}


int main(int argc, char **argv) {
        testmap_Fib();
        testpmap_Fib();
        testmap_RCG();
        testpmap_RCG();
	return 0;
}
