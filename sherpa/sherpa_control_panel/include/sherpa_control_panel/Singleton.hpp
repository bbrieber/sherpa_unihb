#ifndef __SHERPA_SINGLETON_H__
#define __SHERPA_SINGLETON_H__

namespace sherpa_control_panel {
template <class T>
class Singleton
{
   public:
     static T& getInstance();

   protected:
     Singleton() {}

   private:
     Singleton( const Singleton& );
};

template <class T>
T& Singleton<T>::getInstance()
{
  static T instanz;
  return instanz;
}
}
#endif
