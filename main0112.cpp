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
    virtual ~IDraw();
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
    p_t aa;
    p_t bb;
  };

  void make_f(IDraw** b, size_t k);

  void extend(p_t** ps, size_t s, p_t p){
    size_t upd_s = s + 1;
    p_t* res = new p_t [upd_s];
    for (size_t i = 0; i < s; i++){
      res[i] = (*ps)[i];
    }
    res[s] = p;
    delete[] *ps;
    }

    void append(const IDraw* sh, p_t** ppts, size_t& s) {
    extend(ppts, s, sh->begin());
    p_t b = sh->begin();
    while (sh->next(b) != sh->begin()) {
      b = sh->next(b);
      extend(ppts, s, b);
    }
  }

  size_t get_points(IDraw& d, p_t** ps, size_t & s){
    p_t p = d.begin();
    extend(ps,s,p);
    size_t delta = 1;

    while (d.next(p) != d.begin()){
      p = d.next(p);
      extend(ps, s + delta, p);
      ++delta;
    }
    return delta;
  } 

  frame_t build_frame(const p_t*pts, size_t s){
    if (!s){
      throw std::logic_error("bad size");
    }
    int minx = pts[0].x;int maxx = minx;
    int miny = pts[0].y;int  maxy = miny;
    for(size_t i = 1; i < s; i++){
      minx = std::min(minx,pts[i].x);
      maxx = std::max(maxx,pts[i].x);
      miny = std::min(miny,pts[i].y);
      maxy = std::max(maxy,pts[i].y);
    }
    p_t aa{minx, miny};
    p_t bb{maxx, maxy};
    return {aa, bb};
  }

  size_t rows(frame_t fr){
    return (fr.bb.y - fr.aa.y + 1);
  }

  size_t cols(frame_t fr){
    return (fr.bb.x - fr.aa.x + 1);
  }
  char* build_canvas(frame_t fr, char fill){
    char* cnv = new char (rows(fr) * cols(fr));
    for ( size_t i = 0; i < rows(fr); ++i ){
      cnv[i] = fill;
  
    }
    return cnv;
  }

  void point_canvas(char* cnv, frame_t fr, const p_t ps, char f){
    int dx = ps.x - fr.aa.x;
    int dy = fr.bb.y - ps.y;
    cnv[dy * cols(fr) + dx] = f;
  }

  void print_canvas(std :: ostream& os, const char* cnv, frame_t fr){
    for(size_t i = 0; i < rows(fr); i++){
      for(size_t j = 0; j < cols(fr); j++){
        os << cnv[i * cols(fr) + j];
      }
      os << "\n";
    }
  }
}

int main(){
using namespace top;
  int err = 0;
  IDraw* shp[3] = {};
  p_t * pts = nullptr;
  size_t s = 0;
  try {
    shp[0] = new Dot({0, 0});
    shp[1] = new Dot({2, 3});
    shp[2] = new Dot({-5, -2});
    for (size_t i = 0; i < 3; ++i) {
      append(shp[i], &pts, s);
    }
    frame_t fr = build_frame(pts, s);
    char * cnv = build_canvas(fr, '.');
    for (size_t i = 0; i < s; ++i) {
      point_canvas(cnv, fr, pts[i], '#');
    }
    print_canvas(std::cout, cnv, fr);
    delete [] cnv;
  } catch (...) {
    std::cerr << "Error!\n";
    err = 1;
  }
  delete shp[2];
  delete shp[1];
  delete shp[0];
  return err;
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
