#ifndef _MEPHISTO_PROCESSUS_HPP_INCLUDED
#define _MEPHISTO_PROCESSUS_HPP_INCLUDED

namespace mephisto {
	class Processus
	{
	public:
		Processus() {
		}
		~Processus() {
		}

		virtual void start() = 0;
		virtual void update() = 0;
		virtual void stop() = 0;
	};
}

#endif