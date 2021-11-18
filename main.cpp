#include <iostream>
#include <tuple>
#include <vector>
#include <boost/container/flat_set.hpp>

uint32_t f(uint32_t a, uint32_t b){
    return a+b;
}

inline boost::container::flat_set<uint32_t> intersect(const boost::container::flat_set<uint32_t>& set0,
                                                      const boost::container::flat_set<uint32_t>& set1){

    typedef boost::container::flat_set<uint32_t> flat_set;
    flat_set intersection;

    flat_set::const_iterator its[2]={set0.begin(),set1.begin()};
    flat_set::const_iterator ends[2]={set0.end(), set1.end()};
    int current=0;

    while(true){
        while(*its[current]<=*its[1-current]){
            if(*its[current]==*its[1-current]){
                intersection.insert(intersection.end(),*its[current]);
            }


            ++its[current];
            if(its[current]==ends[current]){
                return intersection;
            }

        }
        current=1-current;
    }
}

int main() {

//    std::vector<uint32_t> c={2,4,1};
//    std::vector<uint32_t> a={3,1};
//    std::vector iter={a.begin(),c.begin()};
//
//    for(auto it:iter){
//        std::cout<<*it<<"\n";
//    }


//    std::tuple<uint32_t ,uint32_t > pair=std::make_tuple(a,b);
//    std::cout<<std::apply(f,pair)<<"\n";

    typedef boost::container::flat_set<uint32_t> flat_set;
    flat_set set1,set2;
//    using boost::container::flat_set;
//    flat_set<uint32_t> set1,set2;
    set1.insert(2);
    set1.insert(3);
    set1.insert(4);
    set2.insert(2);
    set2.insert(4);
    set2.insert(5);
    auto set= intersect(set1,set2);
    std::cout<<"Set size: "<<set.size()<<"\n";
//    set1.insert(boost::container::ordered_unique_range, set2.begin(),set2.end());
//    std::cout << "Hello, World! " << set1.size()<< std::endl;
    for(auto i=set.begin();
    i!=set.end();
    ++i){
        std::cout<<*i<<"\n";
    }
    return 0;
}
