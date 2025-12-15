#include <iostream>
#include "geom.hpp"
namespace topit {
  

  struct IDraw {
    virtual ~IDraw() = default;
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
  };

  struct Dot: IDraw {
    Dot(int x, int y);
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t) const override;
    p_t d;
  };
  void extend(p_t** pts, size_t s, p_t p);
  size_t points(const IDraw& d, p_t** pts, size_t s);
  f_t frame(const p_t* pts, size_t s);
  char * canvas(f_t fr, char fill);
  void paint(char* cnv, f_t fr, p_t p, char fill);
  void flush(std::ostream& os, const char* cnv, f_t fr);
}
int main() {
  using topit::IDraw;
  using topit::Dot;
  using topit::f_t;
  using topit::p_t;
  int err = 0;
  IDraw* shps[3] = {};
  p_t * pts = nullptr;
  size_t s = 0;
  try {
    shps[0] = new Dot(0, 0);
    shps[1] = new Dot(5, 7);
    shps[2] = new Dot(-5, -2);
    for (size_t i = 0; i < 3; ++i) {
      s += points(*(shps[i]), &pts, s);
    }
    f_t fr = frame(pts, s);
    char * cnv = canvas(fr, '.');
    for (size_t i = 0; i < s; ++i) {
      paint(cnv, fr, pts[i], '#');
    }
    flush(std::cout, cnv, fr);
    delete [] cnv;
  } catch (...) {
    err = 2;
    std::cerr << "Bad drawing\n";
  }
  delete [] pts;
  delete shps[0];
  delete shps[1];
  delete shps[2];
  return err;
}
void topit::flush(std::ostream& os, const char* cnv, f_t fr) {
  for (size_t i = 0; i < rows(fr); ++i) {
    for (size_t j = 0; j < cols(fr); ++j) {
      os << cnv[i * cols(fr) + j];
    }
    os << "\n";
  }
}
void topit::paint(char* cnv, f_t fr, p_t p, char fill) {
  size_t dy = fr.bb.y - p.y;
  size_t dx = p.x - fr.aa.x;
  cnv[dy * cols(fr) + dx] = fill;
}
size_t topit::rows(f_t fr) {
  return fr.bb.y - fr.aa.y + 1;
}
size_t topit::cols(f_t fr) {
  return fr.bb.x - fr.aa.x + 1;
}
char* topit::canvas(f_t fr, char fill) {
  char* cnv = new char[rows(fr) * cols(fr)];
  for (size_t i = 0; i < rows(fr) * cols(fr); ++i) {
    cnv[i] = fill;
  }
  return cnv;
}
topit::f_t topit::frame(const p_t* pts, size_t s) {
  if (!s) {
    throw std::logic_error("no pts");
  }
  int minx = pts[0].x, maxx = minx;
  int miny = pts[0].y, maxy = miny;
  for (size_t i = 1; i < s; ++i) {
    minx = std::min(minx, pts[i].x);
    maxx = std::max(maxx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t aa = {minx, miny};
  p_t bb = {maxx, maxy};
  return {aa, bb};
}
void topit::extend(p_t** pts, size_t s, p_t p) {
  p_t* e = new p_t[s + 1];
  for (size_t i = 0; i < s; ++i) {
    e[i] = (*pts)[i];
  }
  e[s] = p;
  delete [] *pts;
  *pts = e;
}
size_t topit::points(const IDraw& d, p_t** pts, size_t s) {
  size_t r = 1;
  p_t p = d.begin();
  extend(pts, s, p);
  while (d.next(p) != d.begin()) {
    p = d.next(p);
    extend(pts, s + r, p);
    ++r;
  }
  return r;
}

topit::Dot::Dot(p_t dd):
 IDraw(),
 d{dd}
{}
topit::Dot::Dot(int x, int y):
 IDraw(),
 d{x, y}
{}
topit::p_t topit::Dot::begin() const {
  return d;
}
topit::p_t topit::Dot::next(p_t prev) const {
  if (prev != begin()) {
    throw std::logic_error("bad impl");
  }
  return d;
}
bool topit::operator==(p_t a, p_t b) {
  return a.x == b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b) {
  return !(a == b);
}