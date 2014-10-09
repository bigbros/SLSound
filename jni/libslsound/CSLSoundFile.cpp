#include "CSLSoundFile.h"


CSLSoundFactory	*	CSLSoundFactory::m_begin = 0;
CSLSoundFactory	*	CSLSoundFactory::m_end = 0;


CSLSoundFile::CSLSoundFile(const char * fileName)
: m_fd(-1)
, m_rate(0)
, m_channels(0)
, m_depth(0)
, m_block(0)
, m_length(0)
, m_packet(0)
, m_error(0)
, m_ready(true)
{
	m_fd = open(fileName, O_RDONLY);
	if(m_fd < 0) {
		m_error = errno;
		m_ready = false;
		return;
	}
}

CSLSoundFile::~CSLSoundFile()
{
	if(m_fd >= 0) close(m_fd);
}



CSLSoundFactory::CSLSoundFactory(
		const char * suffix,
		bool (*func_init)(),
		void (*func_exit)(),
		CSLSoundFile * (*func_create)(const char * fileName))
: m_func_init(func_init)
, m_func_exit(func_exit)
, m_func_create(func_create)
, m_prev(0)
, m_next(0)
, m_ready(false)
, m_suffix(suffix)
{
	m_suffix_length = strlen(suffix);
	m_prev = m_end;
	if(m_prev) {
		m_prev->m_next = this;
	} else {
		m_begin = this;
	}
	m_end = this;
}
CSLSoundFactory::~CSLSoundFactory()
{
	if(m_prev) {
		m_prev->m_next = m_next;
	} else {
		m_begin = m_next;
	}
	if(m_next) {
		m_next->m_prev = m_prev;
	} else {
		m_end = m_prev;
	}
}

bool
CSLSoundFactory::Init()
{
	bool result = true;
	for(CSLSoundFactory * factory = m_begin; factory; factory = factory->m_next) {
		if(factory->m_ready) continue;
		factory->m_ready = (factory->m_func_init) ? factory->m_func_init() : true;
		result = result && factory->m_ready;
	}
	return result;
}

void
CSLSoundFactory::Exit()
{
	for(CSLSoundFactory * factory = m_begin; factory; factory = factory->m_next) {
		if(factory->m_func_exit) factory->m_func_exit();
	}
}

CSLSoundFile *
CSLSoundFactory::Create(const char * fileName)
{
	int length = strlen(fileName);
	for(CSLSoundFactory * factory = m_begin; factory; factory = factory->m_next) {
		if(!strcmp(fileName + length - factory->m_suffix_length, factory->m_suffix)) {
			CSLSoundFile * sndFile = factory->m_func_create(fileName);
			return sndFile;
		}
	}
	return 0;
}
