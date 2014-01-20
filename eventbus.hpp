#ifndef EVENTBUS_H_
#define EVENTBUS_H_

#include <string>
#include <vector>

#include <boost/signals2.hpp>

#include "common/events.h"

template <class T>
class singleton {
public:
  template <typename... Args>
  static
  T* get_instance(Args... args) {
	if (!m_instance) {
	  m_instance = new T(std::forward<Args>(args)...);
	}

	return m_instance;
  }

  static
  void destroy_instance() {
	delete m_instance;
	m_instance = nullptr;
  }

private:
  static T* m_instance;
};

template <class T> T*  singleton<T>::m_instance = nullptr;

class event {

};

class event_bus : public singleton<event_bus> {
public:
  event_bus() {
  }

  virtual ~event_bus() {
  }

  template<typename T>
  boost::signals2::signal<T>* createSignal() {
	typedef boost::signals2::signal<T> Signal;

	if (m_signals.find (typeid (T).name()) == m_signals.end ()) {
	  Signal* signal = new Signal();
	  m_signals[typeid (T).name()] = signal;
	  return (signal);
	}
	return (0);
  }

  template<typename T>
  boost::signals2::connection subscribe(const boost::function<T>& callback) {
	typedef boost::signals2::signal<T> Signal;
	Signal* signal = nullptr;

	if (m_signals.find(typeid(T).name()) == m_signals.end()) {
//	  MY_LOGE("no callback for type: %s", typeid (T).name());
	  signal = createSignal<T>();
	} else {
	  signal = dynamic_cast<Signal*> (m_signals[typeid(T).name()]);
	}

	boost::signals2::connection ret = signal->connect(callback);

	m_connections[typeid(T).name()].push_back (ret);
	m_shared_connections[typeid(T).name()].push_back (boost::signals2::shared_connection_block (m_connections[typeid (T).name ()].back(), false));
//    signalsChanged ();
	return (ret);
  }

  template <typename T, typename... Args>
  void post(Args... args) {
    typedef boost::signals2::signal<T> Signal;
    if (m_signals.find(typeid(T).name()) != m_signals.end()) {
      Signal* signal = dynamic_cast<Signal*> (m_signals[typeid(T).name()]);
      signal->operator()(std::forward<Args>(args)...);
    }
  }

private:
  std::map<std::string, boost::signals2::signal_base*> m_signals;
  std::map<std::string, std::vector<boost::signals2::connection> > m_connections;
  std::map<std::string, std::vector<boost::signals2::shared_connection_block> > m_shared_connections;
};

#endif /* EVENTBUS_H_ */
