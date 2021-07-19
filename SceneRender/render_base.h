#pragma once

class core;

class render_base
{
protected:
  core *our_core;
  render_base(){}
public:
  void SetCore(core *new_core)
  {
    our_core = new_core;
  };
  
  virtual void init(void) {};
  virtual void draw(void) {};
};