#include <iostream>

namespace tarasenko
{
  struct point_t
  {
    double x, y;
  };

  struct rectangle_t
  {
    double width, height;
    point_t pos;
  };

  point_t left_bot_rect(rectangle_t);
  point_t right_top_rect(rectangle_t);
  void expand_total_rect(point_t &, point_t &, rectangle_t);
  void print_rect(rectangle_t);

  class Shape
  {
    public:
      virtual ~Shape() = default;
      virtual double getArea() const = 0;
      virtual rectangle_t getFrameRect() const = 0;
      virtual void move(point_t dest) = 0;
      virtual void move(double dx, double dy) = 0;
      virtual void scale(double coeff) = 0;
  };

  class Rectangle: public Shape
  {
    public:
      Rectangle(rectangle_t rect);
      Rectangle(point_t left_bot, point_t right_top);
      double getArea() const override;
      rectangle_t getFrameRect() const override;
      void move(point_t dest) override;
      void move(double dx, double dy) override;
      void scale(double coeff) override;
    private:
      double width, height;
      point_t center;
  };

  class Square: public Shape
  {
    public:
      Square(rectangle_t rect);
      Square(point_t left_bot, double len);
      double getArea() const override;
      rectangle_t getFrameRect() const override;
      void move(point_t dest) override;
      void move(double dx, double dy) override;
      void scale(double coeff) override;
    private:
      double size;
      point_t center;
  };
}

int main()
{
  using namespace tarasenko;
  size_t n = 4;
  Shape** figures = new Shape*[n];
  figures[0] = new Rectangle({6, 7, {11, 2}});
  figures[1] = new Rectangle({1, 2}, {4, 7});
  figures[2] = new Square({5, 2, {0, 0}});
  figures[3] = new Square({2, -1}, 10);
  double total_area = 0;
  for (size_t i = 0; i < n; ++i)
  {
    double current = figures[i]->getArea();
    std::cout << "area " << i << ": " << current << '\n';
    total_area += current;
  }
  std::cout << "total area: " << total_area << '\n';
  point_t left_bot = left_bot_rect(figures[0]->getFrameRect());
  point_t right_top = right_top_rect(figures[0]->getFrameRect());
  for (size_t i = 0; i < n; ++i)
  {
    rectangle_t current = figures[i]->getFrameRect();
    std::cout << "frame rectangle " << i << ":\n";
    print_rect(current);
    expand_total_rect(left_bot, right_top, current);
  }
  point_t total_center = {(right_top.x + left_bot.x) / 2, (right_top.y + left_bot.y) / 2};
  rectangle_t total_rect = {right_top.x - left_bot.x, right_top.y - left_bot.y, total_center};
  std::cout << "total frame rectangle:\n";
  print_rect(total_rect);
}

tarasenko::Rectangle::Rectangle(rectangle_t rect):
  width(rect.width),
  height(rect.height),
  center(rect.pos)
{}

tarasenko::Rectangle::Rectangle(point_t left_bot, point_t right_top):
  width(right_top.x - left_bot.x),
  height(right_top.y - left_bot.y),
  center({(right_top.x + left_bot.x) / 2, (right_top.y + left_bot.y) / 2})
{}

double tarasenko::Rectangle::getArea() const
{
  return width * height;
}

tarasenko::rectangle_t tarasenko::Rectangle::getFrameRect() const
{
  return {width, height, {center.x, center.y}};
}

void tarasenko::Rectangle::move(point_t dest)
{
  center = dest;
}

void tarasenko::Rectangle::move(double dx, double dy)
{
  center.x += dx;
  center.y += dy;
}

void tarasenko::Rectangle::scale(double coeff)
{
  width *= coeff;
  height *= coeff;
}

tarasenko::point_t tarasenko::left_bot_rect(rectangle_t rect)
{
  return {rect.pos.x - (rect.width / 2), rect.pos.y - (rect.height / 2)};
}

tarasenko::point_t tarasenko::right_top_rect(rectangle_t rect)
{
  return {rect.pos.x + (rect.width / 2), rect.pos.y + (rect.height / 2)};
}

void tarasenko::expand_total_rect(point_t & left_bot, point_t & right_top, rectangle_t current)
{
  left_bot.x = left_bot_rect(current).x < left_bot.x ? left_bot_rect(current).x : left_bot.x;
  left_bot.y = left_bot_rect(current).y < left_bot.y ? left_bot_rect(current).y : left_bot.y;
  right_top.x = right_top_rect(current).x > right_top.x ? right_top_rect(current).x : right_top.x;
  right_top.y = right_top_rect(current).y > right_top.y ? right_top_rect(current).y : right_top.y;
}

void tarasenko::print_rect(rectangle_t rect)
{
  std::cout << "  width: " << rect.width << "; height: " << rect.height << '\n';
  std::cout << "  center: " << rect.pos.x << ' ' << rect.pos.y << '\n';
}

tarasenko::Square::Square(rectangle_t rect):
  size(rect.height > rect.width ? rect.width : rect.height),
  center(rect.pos)
{}

tarasenko::Square::Square(point_t left_bot, double len):
  size(len),
  center({left_bot.x + (len / 2), left_bot.y + (len / 2)})
{}

double tarasenko::Square::getArea() const
{
  return size * size;
}

tarasenko::rectangle_t tarasenko::Square::getFrameRect() const
{
  return {size, size, {center.x, center.y}};
}

void tarasenko::Square::move(point_t dest)
{
  center = dest;
}

void tarasenko::Square::move(double dx, double dy)
{
  center.x += dx;
  center.y += dy;
}

void tarasenko::Square::scale(double coeff)
{
  size *= coeff;
}