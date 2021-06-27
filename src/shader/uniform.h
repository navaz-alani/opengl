#ifndef __UNIFORM_H__
#define __UNIFORM_H__

class Uniform {
  const char *m_name;
public:
  Uniform(const char *name) : m_name{ name } {};
  ~Uniform() = default;

  const char *name() const { return m_name; }

  virtual void setToLocation(const int location = -1) = 0;
};

#endif
