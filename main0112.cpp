#include <iostream>
namespace top{
  struct p_t
  {
    int x;
    int y;
  };


  struct IDraw
  {  
    virtual p_t begin() const = 0 ;
    virtual p_t next(p_t) const = 0;
  };
  
  bool operator==(p_t a, p_t b){
    return a.x == b.x && a.y == b.y;
  }

  bool operator!=(p_t a, p_t b){
    return !(a==b);
  }

  struct Dot:IDraw{
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t o;
    Dot(int x, int y);

  };


  struct frame_t{
    p_t left_bot;
    p_t right_top;
  };

  void make_f(IDraw** b, size_t k);

  void get_points(IDraw* b, p_t** ps, size_t & s);

  frame_t build_frame(const p_t*ps, size_t s);

  char* build_convas(frame_t f);
}

int main(){
  using namespace top;
  IDraw* f[3] = {};
  p_t* p = nullptr;
  size_t s = 0;
  try{
    make_f(f,3);
    for(size_t i = 0; i < 3; ++i){
      get_points(f[i], &p, s);
    }
    frame_t fr = build_frame(p, s);
    char * cnv = build_convas(fr);
  }
  catch(...){

  }
}
top::Dot::Dot(int x, int y):
  IDraw(),
  o{x,y}
  {}

top::p_t top::Dot::begin()const{
  return o;
}

top::p_t top::Dot::next(p_t)const{
  return begin();
}
