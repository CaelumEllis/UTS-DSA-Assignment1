#include <utility>
#include <list>
#include "hash.hpp"
#include "algorithm"
#include <cmath>
#include <cstdio>
#include <iostream>
//I'm not sure if splice was a part of the course, 
//this is what I used/found when trying to figure out how to rebuild elements with buckets organsied correctly https://en.cppreference.com/w/cpp/container/list/splice

HashSet::Iterator HashSet::begin() {
  return elements.begin();
}

HashSet::Iterator HashSet::end() {
  return elements.end();
}

//initialising the default hashset with smallest values (default constructor)
HashSet::HashSet() 
  : bucketSizeIndex(0),
    elements(),
    numElements(0),
    maxLoad(1.0f),
    buckets(sizes[bucketSizeIndex]),
    bucketSizes(sizes[bucketSizeIndex], 0)
  {}

//copying all member variables from other hashset (copy constructor)
HashSet::HashSet(const HashSet& other) 
    : bucketSizeIndex(other.bucketSizeIndex),
    elements(other.elements),
    numElements(other.numElements),
    maxLoad(other.maxLoad),
    buckets(other.buckets.size(), std::make_pair(elements.end(), elements.end())),
    bucketSizes(other.bucketSizes)
  {
    for(std::size_t i = 0; i < buckets.size(); i++){
      
      auto otherFirst = other.buckets[i].first;
      auto otherSecond = other.buckets[i].second;

      if(otherFirst == other.elements.end()){
        continue;
      }

      if (!other.elements.empty() && otherFirst != other.elements.end()){
        auto& nonConstElements = const_cast<std::list<int>&>(other.elements);
        auto otherElementsBegin = nonConstElements.begin();

        auto distanceToFirst = std::distance(otherElementsBegin, otherFirst);
        auto distanceToSecond = std::distance(otherElementsBegin, otherSecond);

        auto thisFirst = std::next(elements.begin(), distanceToFirst);
        auto thisSecond = std::next(elements.begin(), distanceToSecond);

        buckets[i].first = thisFirst;
        buckets[i].second = thisSecond;
      }
    }
  }

HashSet& HashSet::operator=(HashSet other) {
  if(this == &other){
    return *this;
  }
  bucketSizeIndex = other.bucketSizeIndex;
  elements = other.elements;
  numElements = other.numElements;
  maxLoad = other. maxLoad;

  buckets.assign(other.buckets.size(), std::make_pair(elements.end(), elements.end()));

  for(std::size_t i = 0; i < buckets.size(); i++){
    auto otherFirst = other.buckets[i].first;
    auto otherSecond = other.buckets[i].second;

    if(other.elements.empty() == false && otherFirst != other.elements.end()){
      auto& nonConstElements = const_cast<std::list<int>&>(other.elements);
      auto otherElementsBegin = nonConstElements.begin();

      auto distanceToFirst = std::distance(otherElementsBegin, otherFirst);
      auto distanceToSecond = std::distance(otherElementsBegin, otherSecond);

      auto thisFirst = std::next(elements.begin(), distanceToFirst);
      auto thisSecond = std::next(elements.begin(), distanceToSecond);

      buckets[i].first = thisFirst;
      buckets[i].second = thisSecond;
    } 
  }
  
  return *this;
}

//destructor, I'm only using standard library containers, they should clean themselves?
HashSet::~HashSet() {
}

void HashSet::insert(int key) {
  //I need to increment numElements after i check the load factor and potentially rehash
  float nextNumElements = numElements + 1;
  float nextLoad = nextNumElements / bucketCount();
  // Rehash if load factor will be exceeded
  if(nextLoad > maxLoad){  
    bucketSizeIndex++;     
    rehash(sizes[bucketSizeIndex]);
  }
  
  if(contains(key) == true){
    return;
  }

  //SIDE NOTE
  //I CANNOT BELIEVE HAVING THIS BEFORE THE LOADFACTOR CHECK BROKE EVERYTHING
  //well i can believe and understand why, but dumb still (by this i mean ++numElements)
  ++numElements;
  std::size_t index = bucket(key); 
  //Using references to modify the actual bucket
  Iterator& beginIt = buckets[index].first;
  Iterator& endIt = buckets[index].second;

  Iterator it;
  
  if(beginIt == endIt){
      elements.push_back(key);
      beginIt = std::prev(elements.end());
      endIt = std::next(beginIt);
  } else{
      it = elements.insert(beginIt, key);
      beginIt = it;
  }
  ++bucketSizes[index];
}

//returns bool of whether the element exists within the set
bool HashSet::contains(int key) const {
  std::size_t index = bucket(key);
  //Iterators for the start and end of the bucket it would be contained in
  Iterator current = buckets[index].first;
  Iterator end = buckets[index].second;

  //loop until the end of the bucket, return true if element is found before end
  while(current != end){
    if(*current == key){
      return true;
    }
    current++;
  }
  return false;
}

//find and return the iterator of key
HashSet::Iterator HashSet::find(int key) {
  std::size_t index = bucket(key);
  //Iterators for the start and end of the bucket it would be contained in
  Iterator current = buckets[index].first;
  Iterator end = buckets[index].second;
  
  while(current != end) {
    if(*current == key){
      return current;
    }
    current++;
  }
  return elements.end();
}

//remove element of value key,
void HashSet::erase(int key) {
  Iterator it = find(key);
  //if there is a match for the target before it reaches the end of elements, erase
  if(it != elements.end()){
    erase(it);
  }
}

HashSet::Iterator HashSet::erase(HashSet::Iterator it) {
 //if not found return end
  if(it == elements.end()){
    return elements.end();
  }
  
  //Iterators for the start and end of the bucket it would be contained in, i realised my segmentation fauilt on random erases must be due to me 
  //failing to catch a case where I delete the elements which are first or second, causing me to dereference the bucket iterator range upon erasing
  //that was a LOT of debugging
  std::size_t index = bucket(*it);
  Iterator& beginIt = buckets[index].first;
  Iterator& endIt = buckets[index].second;

  //saving next valid iterator
  Iterator savedNextIt = std::next(it); 

   elements.erase(it);
  --numElements;
  --bucketSizes[index];

  if(beginIt == it){
    beginIt = savedNextIt;
  }
  if(endIt == savedNextIt){
    endIt = savedNextIt;
  }
  if(bucketSizes[index] == 0){
    beginIt = endIt = elements.end();
  }
  //return the next it so the caller function can continue from it, as it will be now 'de-referenced'
  return savedNextIt;
}

//increase bucket count and 'rehash' out the elements
void HashSet::rehash(std::size_t newSize) {
  while(bucketSizeIndex + 1 < sizes.size() && sizes[bucketSizeIndex] < newSize) {
    ++bucketSizeIndex;
  }

  std::size_t newBucketCnt = sizes[bucketSizeIndex];
  std::vector<std::list<int>> tempBuckets(newBucketCnt);

  //rehashing elements into tempBuckets which is set to the new bucket Count
  Iterator it = elements.begin();
  while(it != elements.end()){
    int val = *it;
    std::size_t tempIndex = (val % newBucketCnt + newBucketCnt) % newBucketCnt;
    tempBuckets[tempIndex].push_back(val);
    it++;
  }
  //clearing and resizing so that temp can be merged back into it
  elements.clear();
  buckets.clear();
  bucketSizes.clear();
  buckets.resize(newBucketCnt, std::make_pair(elements.end(), elements.end()));
  bucketSizes.assign(newBucketCnt, 0);

  for(std::size_t i = 0; i < newBucketCnt; i++){
    if(!tempBuckets[i].empty()){
      //allows me to add each bucket onto the end of elements, allowing for correct separation/grouping of buckets in elements after rehashing
      std::size_t tempBucketSize = tempBuckets[i].size();
      // Iterator beginIt = elements.end();
      elements.splice(elements.end(), tempBuckets[i]);

      Iterator beginIt = std::prev(elements.end(), tempBucketSize);
      // this should be ensuring i have iterators after rehash???
      buckets[i].first = beginIt;
      buckets[i].second = elements.end();

      bucketSizes[i] = tempBucketSize;
    }
  }
}

//return numElements
std::size_t HashSet::size() const {
  return numElements;
}

//return bool of set is empty
bool HashSet::empty() const {
  return numElements == 0;
}

//return current bucket count
std::size_t HashSet::bucketCount() const {
  return buckets.size();
}

//return number of elements in bucket b
std::size_t HashSet::bucketSize(std::size_t b) const {
  // return std::distance(buckets[b].first, buckets[b].second);
  return bucketSizes[b];
}

//return the correct bucket index for key
std::size_t HashSet::bucket(int key) const {
  std::size_t cnt = bucketCount();
  if(cnt == 0){
    return 0;
  }
  return (key % cnt + cnt) % cnt;
}

//return current load factor
float HashSet::loadFactor() const {
  return (float)numElements / bucketCount();
}

//return current maxLoad set
float HashSet::maxLoadFactor() const {
  return maxLoad;
}

//set new loadFactor and rehash if new loadfactor is less the the old loadfactor
void HashSet::maxLoadFactor(float newMaxLoad) {
  maxLoad = newMaxLoad;
  if(loadFactor() > maxLoad){

    std::size_t requiredSize = static_cast<std::size_t>(numElements / maxLoad) +1;
    //because this was removed from rehash, due to my new logic in my inset for efficiency (automatically incrementing bucketSizeIndex to get the next size)
    //maxload now needs a way to choose its correct size, hence moving it here
    while(bucketSizeIndex + 1 < sizes.size() && sizes[bucketSizeIndex] < requiredSize) {
      ++bucketSizeIndex;
    }
    rehash(sizes[bucketSizeIndex]);
  }
}

//ignore this was just when trying to diagnose my complexity issue with contains/erase, I think my issue must be my insert, or specifically how i check if an element is present, 
//just unsure how I can make that more efficient
void HashSet::printAllBuckets() const {
  for(std::size_t i = 0; i < buckets.size(); i++){
    std::cout << "Bucket " << i << " Containts " << bucketSizes[i] << "Elements";
    // for(Iterator it = buckets[i].first; it != buckets[i].second; it++){
    //   std::cout << *it << " ";
    // }
    std::cout << "\n";
  }
}
