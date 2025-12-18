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
}

int main()
{
  using namespace tarasenko;
  size_t n = 1;
  Shape** figures = new Shape*[n];
  figures[0] = new Rectangle({1, 2}, {4, 7});
  double total_area = 0;
  for (size_t i = 0; i < n; ++i)
  {
    double current = figures[i]->getArea();
    std::cout << "area " << n << ": " << current << '\n';
    total_area += current;
  }
  std::cout << "total area: " << total_area << '\n';
  point_t left_bot = left_bot_rect(figures[0]->getFrameRect());
  point_t right_top = right_top_rect(figures[0]->getFrameRect());
  for (size_t i = 0; i < n; ++i)
  {
    rectangle_t current = figures[i]->getFrameRect();
    std::cout << "frame rectangle " << n << ":\n";
    std::cout << "  width: " << current.width << "; height: " << current.height << '\n';
    std::cout << "  center: " << current.pos.x << ' ' << current.pos.y << '\n';
    left_bot.x = left_bot_rect(current).x < left_bot.x ? left_bot_rect(current).x : left_bot.x;
    left_bot.y = left_bot_rect(current).y < left_bot.y ? left_bot_rect(current).y : left_bot.y;
    right_top.x = right_top_rect(current).x > right_top.x ? right_top_rect(current).x : right_top.x;
    right_top.y = right_top_rect(current).y > right_top.y ? right_top_rect(current).y : right_top.y;
  }
  std::cout << "frame rectangle " << n << ":\n";
  std::cout << "  width: " << right_top.x - left_bot.x << "; height: " << right_top.y - left_bot.y << '\n';
  std::cout << "  center: " << (right_top.x + left_bot.x) / 2 << ' ' << (right_top.y + left_bot.y) / 2 << '\n';
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

tarasenko::point_t tarasenko::left_bot_rect(tarasenko::rectangle_t rect)
{
  return {rect.pos.x - (rect.width / 2), rect.pos.y - (rect.height / 2)};
}

tarasenko::point_t tarasenko::right_top_rect(tarasenko::rectangle_t rect)
{
  return {rect.pos.x + (rect.width / 2), rect.pos.y + (rect.height / 2)};
}