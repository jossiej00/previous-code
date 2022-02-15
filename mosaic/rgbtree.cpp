/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{
for (auto it = photos.begin(); it != photos.end(); it++ )
{
  tree.push_back(it->first);
}
cout<< tree.size() <<endl;
buildTree(0, tree.size()-1, 0);
}


void rgbtree::buildTree(int start, int end, int d){
  int mid = (start+end)/2;
  if(start >= end){
    return;
  }
  quickSelect(start, end, mid, d);
  int left = mid-1;
  int right = mid+1;
  buildTree(start, left, (d+1)%3);
  buildTree(right, end, (d+1)%3);
}

RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{
return findNNHelper(query, 0, tree.size()-1, 0);
}

RGBAPixel rgbtree::findNNHelper(const RGBAPixel & query, int start, int end, int d) const{
//if start >= end return tree[start]
 if(start >= end){
  return tree[start];
 }
  RGBAPixel currBest;
  // find root(median) and make that the current best
  int mid = floor((start+end)/2);
  currBest = tree[mid];

  int currBestDist = distBetween(query, tree[mid]);
  // recursively call to find the best within the subtree
  // similar to binary serach where the start and end position change based on value
  int in_s; int in_e; int out_s; int out_e;
  if(smallerByDim(query, tree[mid], d)){
    in_s = start; in_e = mid-1; out_s = mid+1; out_e = end;
  } else{
    out_s = start; out_e = mid-1; in_s = mid+1; in_e = end;
  }
  
  RGBAPixel curr = findNNHelper(query, in_s ,in_e, (d+1)%3);
  int currDist = distBetween(query, curr);
  // compare that best with currbest and replace if necessary
  if(currDist < currBestDist){
      currBest = curr;
      currBestDist = currDist;
  }
  // check if there is a better point in another subtree(disttosplit)
  if(currBestDist > distToSplit(query, tree[mid], d)){
    // if true go trhough the same process as before
    RGBAPixel curr1 = findNNHelper(query, out_s ,out_e, (d+1)%3);
    int currDist1 = distBetween(query, curr1);
    if(currDist1 < currBestDist){
      currBest = curr1;
      currBestDist = currDist1;
    }
  }
  // return currbest
  return currBest;
}



int rgbtree::distBetween(const RGBAPixel & first, const RGBAPixel & second) const 
{
  return pow((first.r-second.r),2)+pow((first.g-second.g),2)+pow((first.b-second.b),2);
}


bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{
  int firstDim = getColor(first, curDim);
  int secondDim = getColor(second, curDim);
  if(firstDim==secondDim){
    return first.operator<(second);
  } else{
    return firstDim < secondDim;
  }
}





/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{
  if((start>=end)) {
    return;
  } 
  int p = partition(start, end, d);
  if(p<k){
    quickSelect(p+1, end, k, d);
  }else if (p>k){
    quickSelect(start, p-1, k, d);
  }
}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    int p = lo;
    for( int i=lo+1; i <= hi; i++){
      if(getColor(tree[i], d) < getColor(tree[lo], d)) {
        p++; swap(tree[p], tree[i]);
        }
    }
    swap(tree[lo], tree[p]);

    return p;
}

int rgbtree::getColor(const RGBAPixel pixel, int d) const{
  if(d==0){
    return pixel.r;
  } else if(d == 1){
    return pixel.g;
  } else{
    return pixel.b;
  }
}


/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{
  int currPos = getColor(curr, dimension);
  int queryPos = getColor(query, dimension);
  return pow(currPos-queryPos,2);
}

