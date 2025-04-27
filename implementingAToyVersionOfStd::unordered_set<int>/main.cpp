
#include <gtest/gtest.h>
#include <random>
#include <algorithm>
#include <unordered_set>
#include "hash.hpp"

Level 1 Tests
TEST(Level1Test, insertOne) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  const int toInsert {42};
  ASSERT_FALSE(h.contains(toInsert));
  h.insert(toInsert);
  ASSERT_TRUE(h.contains(toInsert));
  const int notPresent {31};
  ASSERT_FALSE(h.contains(notPresent));
  ASSERT_FALSE(h.empty());
}

TEST(Level1Test, insertNegativeNumbers) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  const std::vector<int> negativeNumbers {-10, -32, -38, -101, -1'003};
  for (int x : negativeNumbers) {
    h.insert(x);
    ASSERT_TRUE(h.contains(x));
  }
  ASSERT_EQ(h.size(), negativeNumbers.size());
  ASSERT_FALSE(h.empty());
}

TEST(Level1Test, smallSetTest) {
  HashSet h;
  const int num {90};
  h.insert(num);
  ASSERT_EQ(h.size(), 1u);
  h.insert(num);
  ASSERT_EQ(h.size(), 1u);
}

TEST(Level1Test, noDuplicates) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  const std::vector<int> repeatedNumbers {5, 32, 5, -20, -20, 32, 5};
  for (int x : repeatedNumbers) {
    h.insert(x);
    ASSERT_TRUE(h.contains(x));
  }
  ASSERT_EQ(h.size(), 3u);
  const int notPresent {48};
  ASSERT_FALSE(h.contains(notPresent));
  ASSERT_FALSE(h.empty());
}

TEST(Level1Test, largerSetTest) {
  std::mt19937 mt {483'238'398};
  std::uniform_int_distribution<int> dist {-100, 100};
  HashSet h;
  h.maxLoadFactor(0.5);
  std::unordered_set<int> stlh;
  for (int i = 0; i < 150; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
    ASSERT_EQ(h.size(), stlh.size());
  }
}

TEST(Level1Test, insertAndErase) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  const int num {42};
  h.insert(num);
  ASSERT_TRUE(h.contains(num));
  const int notPresent {67};
  ASSERT_FALSE(h.contains(notPresent));
  ASSERT_FALSE(h.empty());
  ASSERT_EQ(h.size(), 1u);
  h.erase(num);
  ASSERT_TRUE(h.empty());
  ASSERT_FALSE(h.contains(num));
}

TEST(Level1Test, insertAndEraseAndInsert) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  const int num1 {27};
  const int num2 {12};
  h.insert(num1);
  h.insert(num2);
  ASSERT_TRUE(h.contains(num1));
  ASSERT_TRUE(h.contains(num2));
  const int notPresent1 {num1 + static_cast<int>(h.bucketCount())};
  const int notPresent2 {num2 + static_cast<int>(h.bucketCount())};
  ASSERT_FALSE(h.contains(notPresent1));
  ASSERT_FALSE(h.contains(notPresent2));
  ASSERT_FALSE(h.empty());
  ASSERT_EQ(h.size(), 2u);
  h.erase(num1);
  ASSERT_FALSE(h.contains(num1));
  ASSERT_TRUE(h.contains(num2));
  h.erase(num2);
  ASSERT_FALSE(h.contains(num2));
  h.insert(num2);
  ASSERT_TRUE(h.contains(num2));
  ASSERT_FALSE(h.contains(num1));
  ASSERT_EQ(h.size(), 1u);
}

TEST(Level1Test, toggleEmptyAnd1) {
  HashSet h;
  std::mt19937 mt {2'823'983};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  for (int i = 0; i < 100; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    ASSERT_TRUE(h.contains(elem));
    ASSERT_FALSE(h.empty());
    h.erase(elem);
    ASSERT_FALSE(h.contains(elem));
    ASSERT_TRUE(h.empty());
  }
}

TEST(Level1Test, copyEmptyHashSet) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  HashSet h2 {h};
  ASSERT_TRUE(h2.empty());
  const int num = -35;
  h2.insert(num);
  ASSERT_TRUE(h.empty());
  ASSERT_FALSE(h.contains(num));
  ASSERT_TRUE(h2.contains(num));
  ASSERT_FALSE(h2.empty());
}

TEST(Level1Test, copyConstructor) {
  HashSet h;
  ASSERT_FALSE(h.contains(42));
  std::mt19937 mt {990'322'389};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  for (int i = 0; i < 100; i++) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  HashSet h2 {h};
  ASSERT_EQ(h.size(), stlh.size());
  ASSERT_EQ(h.size(), h2.size());
  for (int x : stlh) {
    ASSERT_TRUE(h2.contains(x));
  }
}

TEST(Level1Test, assignOneElement) {
  HashSet h;
  const int num {-89};
  h.insert(num);
  ASSERT_FALSE(h.empty());
  HashSet h2;
  ASSERT_TRUE(h2.empty());
  h2 = h;
  ASSERT_FALSE(h2.empty());
  ASSERT_TRUE(h2.contains(num));
  const int notIn {-38};
  h2.insert(notIn);
  ASSERT_FALSE(h.contains(notIn));
}

TEST(Level1Test, assignmentOperator) {
  HashSet h;
  ASSERT_FALSE(h.contains(42));
  std::mt19937 mt {9'833'899};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  HashSet h2;
  for (int i = 0; i < 100; i++) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
    h2.insert(elem % 5'000);
  }
  const int num {6'871};
  h.insert(num);
  ASSERT_FALSE(h2.contains(num));
  h2 = h;
  ASSERT_EQ(h2.size(), h.size());
  ASSERT_TRUE(h2.contains(num));
  for (int x : stlh) {
    ASSERT_TRUE(h2.contains(x));
  }
}

TEST(Level1Test, setMaxLoadFactor) {
  HashSet h;
  float threshold = 0.5;
  h.maxLoadFactor(threshold);
  ASSERT_FLOAT_EQ(h.maxLoadFactor(), threshold);
  threshold = 0.75;
  h.maxLoadFactor(threshold);
  ASSERT_FLOAT_EQ(h.maxLoadFactor(), threshold);
}

TEST(Level1Test, loadFactor) {
  HashSet h;
  std::mt19937 mt {6'317'237};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  for (int i = 0; i < 1'000; i++) {
    h.insert(dist(mt));
  }
  float factor = h.loadFactor();
  float actual = static_cast<float>(h.size()) /
                 static_cast<float>(h.bucketCount());
  EXPECT_FLOAT_EQ(factor, actual);
}

TEST(Level1Test, enoughBuckets) {
  HashSet h;
  const float threshold = 0.5;
  h.maxLoadFactor(threshold);
  std::mt19937 mt {2'389'239};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  for (int i = 0; i < 1'000; i++) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  ASSERT_GE(h.bucketCount(), 2 * h.size());
}

TEST(Level1Test, setLoadFactorProvokesRehash) {
  HashSet h;
  ASSERT_TRUE(h.empty());
  float threshold = 1.0;
  h.maxLoadFactor(threshold);
  std::mt19937 mt {89'338'157};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  for (int i = 0; i < 1'000; i++) {
    int elem = dist(mt);
    h.insert(elem);
  }
  ASSERT_FALSE(h.empty());
  std::size_t initialSize = h.size();
  threshold = 0.5;
  h.maxLoadFactor(threshold);
  ASSERT_GE(h.bucketCount(), 2 * initialSize);
}

TEST(Level1Test, primeSizes) {
  const std::vector<std::size_t> sizes {
    1ul, 13ul, 59ul, 127ul, 257ul, 541ul, 1'109ul, 2'357ul,
      5'087ul, 10'273ul, 20'753ul, 42'043ul};
  HashSet h;
  float threshold = 1.0;
  h.maxLoadFactor(threshold);
  std::unordered_set<int> stlh;
  std::mt19937 mt {8'342'389};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  for (int i = 0; i < 10'000; i++) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
    std::size_t currentBuckets = h.bucketCount();
    auto it = std::find(sizes.begin(), sizes.end(), currentBuckets);
    ASSERT_NE(it, sizes.end());
  }
  ASSERT_EQ(h.size(), stlh.size());
  ASSERT_GE(h.bucketCount(), h.size());
}

TEST(Level1Test, eraseAfterRehash) {
  HashSet h;
  std::mt19937 mt {989'823};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  for (int i = 0; i < 10'000; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
    h.erase(x);
    ASSERT_FALSE(h.contains(x));
  }
}

TEST(Level1Test, bucketsNotTooFull) {
  HashSet h;
  h.maxLoadFactor(0.5);
  for (int i = 0; i < 100; ++i) {
    h.insert(i);
  }
  for (int i = 0; i < 100; ++i) {
    std::size_t b = h.bucket(i);
    ASSERT_LE(h.bucketSize(b), 1u);
  }
  const int num {32};
  std::size_t bucket {h.bucket(num)};
  ASSERT_EQ(h.bucketSize(bucket), 1u);
}

TEST(Level1Test, bucketTest) {
  HashSet h;
  h.maxLoadFactor(0.5);
  std::mt19937 mt {483'238'398};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::vector<int> values(5);
  std::generate(values.begin(), values.end(),
      [&mt, &dist](){return dist(mt);});
  for (int x : values) {
    h.insert(x);
  }
  for (int x : values) {
    int b = h.bucket(x);
    ASSERT_GE(h.bucketSize(b), 1u);
  }
}

TEST(Level1Test, correctSizeWithErase) {
  std::mt19937 mt {8'329'822};
  std::uniform_int_distribution<int> dist {-1'000, 1'000};
  HashSet h;
  std::unordered_set<int> stlh;
  for (int i = 0; i < 100; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (std::size_t i = 0; i < stlh.size(); ++i) {
    auto it = stlh.begin();
    ASSERT_TRUE(h.contains(*it));
    h.erase(*it);
    ASSERT_FALSE(h.contains(*it));
    stlh.erase(it);
    ASSERT_EQ(h.size(), stlh.size());
  }
}

TEST(Level1Test, randomContains) {
  std::mt19937 mt {281'777};
  std::uniform_int_distribution<int> dist {-100, 100};
  HashSet h;
  std::unordered_set<int> stlh;
  const int N {100};
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    ASSERT_EQ(h.contains(elem), stlh.contains(elem));
  }
}

TEST(Level1Test, randomErases) {
  std::mt19937 mt {281'777};
  std::uniform_int_distribution<int> dist {-100, 100};
  HashSet h;
  std::unordered_set<int> stlh;
  const int N {100};
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  for (int i = 0; i < N/2; ++i) {
    int elem = dist(mt);
    h.erase(elem);
    stlh.erase(elem);
  }

  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);

    ASSERT_EQ(h.contains(elem), stlh.contains(elem));
  }

}

TEST(Level1Test, containsComplexity) {
  std::mt19937 mt {13'884};
  std::uniform_int_distribution<int> dist;
  HashSet h;
  ASSERT_TRUE(h.empty());
  ASSERT_FALSE(h.contains(42));
  std::vector<int> vec;
  const int N {100'000};
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    vec.push_back(elem);
  }
  
  std::shuffle(vec.begin(), vec.end(), mt);
  
  // h.printAllBuckets();
  
  for (int x : vec) {
    ASSERT_TRUE(h.contains(x));
  }
}

TEST(Level1Test, eraseComplexity) {
  std::mt19937 mt {82'323'893};
  std::uniform_int_distribution<int> dist;
  HashSet h;
  std::vector<int> vec;
  const int N {100'000};
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    if (not h.contains(elem)) {
      vec.push_back(elem);
    }
    h.insert(elem);
  }
  ASSERT_EQ(h.size(), vec.size());
  std::shuffle(vec.begin(), vec.end(), mt);
  for (int x : vec) {
    h.erase(x);
  }
  ASSERT_TRUE(h.empty());
}

// Level 2 Tests Start Here
TEST(Level2Test, whenEmptyBeginIsEnd) {
  HashSet h;
  ASSERT_EQ(h.begin(), h.end());
  h.insert(-43);
  ASSERT_NE(h.begin(), h.end());
}

TEST(Level2Test, oneElementBegin) {
  HashSet h;
  const int num {1'898};
  h.insert(num);
  auto it = h.begin();
  ASSERT_EQ(*it, num);
}

TEST(Level2Test, oneElementFind) {
  HashSet h;
  const int num {23'892'398};
  h.insert(num);
  auto it = h.find(num);
  ASSERT_EQ(*it, num);
  it = h.find(num + 1);
  ASSERT_EQ(it, h.end());
}

TEST(Level2Test, oneElementIteratorErase) {
  HashSet h;
  const int num {-238'923};
  h.insert(num);
  ASSERT_TRUE(h.contains(num));
  auto it = h.begin();
  auto next = h.erase(it);
  ASSERT_FALSE(h.contains(num));
  ASSERT_EQ(next, h.end());
}

TEST(Level2Test, findSameBucket) {
  HashSet h;
  const std::vector<int> toInsert {14, 27, 40, 53};
  for (int x : toInsert) {
    h.insert(x);
  }
  for (int x : toInsert) {
    auto it = h.find(x);
    ASSERT_EQ(*it, x);
  }
  const int num {toInsert.back() + static_cast<int>(h.size())};
  h.insert(num);
  auto it = h.find(num);
  ASSERT_EQ(*it, num);
}

TEST(Level2Test, eraseAtEnd) {
  HashSet h;
  h.insert(32);
  h.insert(45);
  auto it = h.end();
  std::advance(it, -1);
  auto returnedIt = h.erase(it);
  ASSERT_EQ(h.size(), 1u);
  ASSERT_EQ(returnedIt, h.end());
}

TEST(Level2Test, eraseIteratorAtBeginning) {
  HashSet h;
  h.insert(32);
  h.insert(45);
  ASSERT_EQ(h.size(), 2u);
  auto it = h.begin();
  auto it2 = it;
  ++it2;
  auto next = h.erase(it);
  ASSERT_EQ(h.size(), 1u);
  ASSERT_EQ(next, it2);
  auto it3 = h.erase(next);
  ASSERT_EQ(it3, h.end());
  ASSERT_TRUE(h.empty());
}

TEST(Level2Test, eraseSameBucket) {
  HashSet h;
  std::unordered_set<int> stlh;
  const std::vector<int> toInsert {14, 27, 40, 53};
  for (int x : toInsert) {
    h.insert(x);
    stlh.insert(x);
  }
  const int num {toInsert.back() + static_cast<int>(h.size())};
  h.insert(num);
  stlh.insert(num);
  auto numIt = h.find(num);
  ASSERT_EQ(*numIt, num);
  for (auto it = toInsert.rbegin(); it != toInsert.rend(); ++it) {
    h.erase(*it);
    stlh.erase(*it);
    ASSERT_EQ(h.size(), stlh.size());
    ASSERT_FALSE(h.contains(*it));
  }
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
  }
  for (int x : h) {
    ASSERT_TRUE(stlh.contains(x));
  }
}

TEST(Level2Test, incrementIterator) {
  std::vector<int> vec {-89, 828, -812, -889, 8, 89};
  HashSet h;
  for (int x : vec) {
    h.insert(x);
  }
  std::vector<int> copy;
  for (auto it = h.begin(); it != h.end(); ++it) {
    copy.push_back(*it);
  }
  std::sort(vec.begin(), vec.end());
  std::sort(copy.begin(), copy.end());
  ASSERT_EQ(copy, vec);
}

TEST(Level2Test, decrementIterator) {
  std::vector<int> vec {328, 892, 34, -99, -18, 83};
  HashSet h;
  for (int x : vec) {
    h.insert(x);
  }
  std::vector<int> copy;
  for (auto it = h.end(); it != h.begin();) {
    --it;
    copy.push_back(*it);
  }
  std::sort(vec.begin(), vec.end());
  std::sort(copy.begin(), copy.end());
  ASSERT_EQ(copy, vec);
}

TEST(Level2Test, iteratorValidAfterRehash) {
  HashSet h;
  float threshold = 1.0;
  h.maxLoadFactor(threshold);
  std::mt19937 mt {239'823};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  for (int i = 0; i < 120; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  std::uniform_int_distribution<int> indexDist
      {0, static_cast<int>(stlh.size()) - 1};
  int index = indexDist(mt);
  auto stlIt = stlh.begin();
  std::advance(stlIt, index);
  int num = *stlIt;
  auto it = h.find(num);
  ASSERT_NE(it, h.end());
  ASSERT_EQ(*it, num);
  for (int i = 0; i < 150; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  ASSERT_EQ(*it, num);
  threshold = 0.2;
  h.maxLoadFactor(threshold);
  ASSERT_LE(h.loadFactor(), threshold);
  ASSERT_EQ(*it, num);
}

TEST(Level2Test, correctElements) {
  std::unordered_set<int> stlh;
  float threshold = 0.5;
  stlh.max_load_factor(threshold);
  HashSet h;
  h.maxLoadFactor(threshold);
  std::mt19937 mt {83'928'928};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  const int insertionNumber {5'000};
  for (int i = 0; i < insertionNumber; i++) {
    int elem = dist(mt);
    stlh.insert(elem);
    h.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
  }
  std::size_t counter = 0;
  for (int x : h) {
    ++counter;
    ASSERT_TRUE(h.contains(x));
    ASSERT_TRUE(stlh.contains(x));
  }
  ASSERT_EQ(counter, stlh.size());
}

TEST(Level2Test, correctElementsLarger) {
  std::unordered_set<int> stlh;
  float threshold = 0.5;
  stlh.max_load_factor(threshold);
  HashSet h;
  h.maxLoadFactor(threshold);
  std::mt19937 mt {899'889};
  std::uniform_int_distribution<int> dist;
  const int insertionNumber {10'000};
  for (int i = 0; i < insertionNumber; i++) {
    int elem = dist(mt);
    stlh.insert(elem);
    h.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
  }
  std::size_t counter = 0;
  for (int x : h) {
    ASSERT_TRUE(h.contains(x));
    ASSERT_TRUE(stlh.contains(x));
    ++counter;
  }
  ASSERT_EQ(counter, stlh.size());
}

TEST(Level2Test, stdFindWorks) {
  HashSet h;
  std::unordered_set<int> stlh;
  std::mt19937 mt {899'889};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  const int insertionNumber {1'000};
  for (int i = 0; i < insertionNumber; i++) {
    int elem = dist(mt);
    stlh.insert(elem);
    h.insert(elem);
  }
  std::uniform_int_distribution<int> indexDist
      {0, static_cast<int>(stlh.size()) - 1};
  int index = indexDist(mt);
  auto stlIt = stlh.begin();
  std::advance(stlIt, index);
  int num = *stlIt;
  auto it = std::find(h.begin(), h.end(), num);
  ASSERT_EQ(*it, num);
}

TEST(Level2Test, randomErase) {
  std::unordered_set<int> stlh;
  float threshold = 0.5;
  stlh.max_load_factor(threshold);
  HashSet h;
  h.maxLoadFactor(threshold);
  std::mt19937 mt {128'381'289};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  for (int i = 0; i < 100; i++) {
    int elem = dist(mt);
    stlh.insert(elem);
    h.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
    auto it = h.find(x);
    ASSERT_EQ(*it, x);
    h.erase(x);
    ASSERT_FALSE(h.contains(x));
  }
}

TEST(Level2Test, find) {
  std::mt19937 mt {328'238'923};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  std::unordered_set<int> stlh;
  HashSet h;
  for (int i = 0; i < 100; i++) {
    int elem = dist(mt);
    stlh.insert(elem);
    h.insert(elem);
  }
  for (int x : stlh) {
    auto it = h.find(x);
    ASSERT_EQ(*it, x);
  }
  auto it = h.find(10'001);
  ASSERT_EQ(it, h.end());
}

TEST(Level2Test, iteratorsValidAfterInsert) {
  std::mt19937 mt {3'819'189};
  std::uniform_int_distribution<int> dist;
  HashSet h;
  for (int i = 0; i < 100; ++i) {
    h.insert(dist(mt));
  }
  auto it = h.begin();
  std::advance(it, 35);
  auto finder = h.find(*it);
  ASSERT_EQ(finder, it);
  ASSERT_NE(finder, h.end());
  for (int i = 0; i < 300; ++i) {
    h.insert(dist(mt));
  }
  ASSERT_EQ(finder, it);
}

TEST(Level2Test, iteratorsValidAfterErase) {
  HashSet h;
  h.maxLoadFactor(1.0);
  const std::vector<int> values {0, 13, 26, 39, 52, 65};
  for (int x : values) {
    h.insert(x);
  }
  auto it = h.find(26);
  ASSERT_EQ(*it, 26);
  h.erase(0);
  h.erase(65);
  ASSERT_FALSE(h.contains(0));
  ASSERT_FALSE(h.contains(65));
  ASSERT_EQ(*it, 26);
}

TEST(Level2Test, iteratorsValidAfterRehash) {
  HashSet h;
  h.maxLoadFactor(1.0);
  std::vector<int> values {0, 13, 26, 39, 52, 65};
  for (int x : values) {
    h.insert(x);
  }
  auto it = h.find(26);
  ASSERT_EQ(*it, 26);
  for (int i = 0; i < 100; ++i) {
    h.insert(i);
  }
  ASSERT_EQ(*it, 26);
}


TEST(Level2Test, eraseAndInsert) {
  std::mt19937 mt {8'329'822};
  std::uniform_int_distribution<int> dist {-1'000, 1'000};
  HashSet h;
  std::unordered_set<int> stlh;
  for (int i = 0; i < 100; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int i = 0; i < 100; ++i) {
    int coin = dist(mt);
    if (coin > 0) {
      auto it = stlh.begin();
      h.erase(*it);
      stlh.erase(it);
    } else {
      h.insert(coin);
      stlh.insert(coin);
    }
    ASSERT_EQ(h.size(), stlh.size());
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
  }
  std::size_t counter = 0;
  for (int x : h) {
    ASSERT_TRUE(stlh.contains(x));
    ++counter;
  }
  ASSERT_EQ(counter, stlh.size());
}

TEST(Level2Test, findComplexity) {
  std::mt19937 mt {99'128};
  std::uniform_int_distribution<int> dist;
  HashSet h;
  std::vector<int> vec;
  const int N {100'000};
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    vec.push_back(elem);
  }
  std::shuffle(vec.begin(), vec.end(), mt);
  for (int x : vec) {
    auto it = h.find(x);
    ASSERT_EQ(*it, x);
  }
}

TEST(Level2Test, eraseComplexityIterator) {
  std::mt19937 mt {82'311};
  std::uniform_int_distribution<int> dist;
  HashSet h;
  std::vector<int> vec;
  const int N {100'000};
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    if (not h.contains(elem)) {
      vec.push_back(elem);
    }
    h.insert(elem);
  }
  ASSERT_EQ(h.size(), vec.size());
  std::shuffle(vec.begin(), vec.end(), mt);
  for (int x : vec) {
    auto it = h.find(x);
    auto next = h.erase(it);
    if (next != h.end()) {
      ASSERT_TRUE(h.contains(*next));
    }
  }
  ASSERT_TRUE(h.empty());
}

TEST(Level2Test, eraseFromBeginningAndEnd) {
  std::mt19937 mt {82'757};
  std::uniform_int_distribution<int> dist {-10'000, 10'000};
  HashSet h;
  for (int i = 0; i < 100; ++i) {
    h.insert(dist(mt));
  }
  std::size_t size {h.size()};
  ASSERT_FALSE(h.empty());
  for (std::size_t i = 0; i < size; ++i) {
    if (i % 2 == 0) {
      auto next = h.erase(h.begin());
      if (next != h.end()) {
        ASSERT_TRUE(h.contains(*next));
      }
    } else {
      auto it = h.end();
      std::advance(it, -1);
      auto last = h.erase(it);
      ASSERT_EQ(last, h.end());
    }
  }
  ASSERT_TRUE(h.empty());
}

TEST(Level2Test, randomFind) {
  std::mt19937 mt {6'276};
  const int N {1'000};
  std::uniform_int_distribution<int> dist {-N, N};
  HashSet h;
  std::unordered_set<int> stlh;
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    if (stlh.contains(elem)) {
      auto it = h.find(elem);
      ASSERT_EQ(*it, elem);
    } else {
      ASSERT_EQ(h.find(elem), h.end());
    }
  }
}

TEST(Level2Test, eraseVersusUnorderedSet) {
  std::mt19937 mt {812'189};
  const int N {1'000};
  std::uniform_int_distribution<int> dist {-N, N};
  HashSet h;
  std::unordered_set<int> stlh;
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.insert(elem);
    stlh.insert(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int i = 0; i < N; ++i) {
    int elem = dist(mt);
    h.erase(elem);
    stlh.erase(elem);
  }
  ASSERT_EQ(h.size(), stlh.size());
  for (int x : stlh) {
    ASSERT_TRUE(h.contains(x));
  }
  std::size_t counter = 0;
  for (int x : h) {
    ASSERT_TRUE(stlh.contains(x));
    ++counter;
  }
  ASSERT_EQ(counter, stlh.size());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
