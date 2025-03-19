#ifndef _FSGE_PROCESSUS_HPP_INCLUDED
#define _FSGE_PROCESSUS_HPP_INCLUDED

namespace fsge {
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