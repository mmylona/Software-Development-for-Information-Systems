//
// Created by snz on 11/6/2023.
//
#include <gtest/gtest.h>
#include "../../../../headers/structures/vector/Vector.h"
#include "../../../../headers/structures/point/Point.h"
TEST(PointTest,CustomHashPoint){
//given
    Vector<float> vec;
    vec.push_back(1.1f);
    vec.push_back(2.2f);
    vec.push_back(3.3f);
    Point p(1,vec);
    //when
    size_t hash = p.customHash();
    //then
    ASSERT_EQ(hash, 30817);
}

TEST(PointTest,HashValuePoint){
//given
    Vector<float> vec;
    vec.push_back(1.1f);
    vec.push_back(2.2f);
    vec.push_back(3.3f);
    Point p(1,vec);
    //when
    size_t hash = p.hashValue();
    //then
    ASSERT_EQ(hash, 30817);

}

TEST(PointTest,SetIdPoint){
//given
    Vector<float> vec;
    vec.push_back(1.1f);
    vec.push_back(2.2f);
    vec.push_back(3.3f);
    Point p(1,vec);
    //when
    p.setId(2);
    //then
    ASSERT_EQ(p.getId(), 2);
}

TEST(PointTest,SetHasBeenChoosenPoint){
    //given
    Vector<float> vec;
    vec.push_back(1.1f);
    vec.push_back(2.2f);
    vec.push_back(3.3f);
    Point p(1,vec);
    //when
    p.setHasBeenChoosen(2);
    //then
    ASSERT_EQ(p.getHasBeenChoosen(), 2);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}