#ifndef CSLSoundFile_h
#define CSLSoundFile_h

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <error.h>

class CSLSoundFile
{
private:
	int				m_error;
protected:
	int				m_fd;		// �ǂݍ��ݒ��̃t�@�C��
	int				m_rate;		// �T���v�����O���[�g(ex. 44100)
	short			m_channels;	// �`���l����
	short			m_depth;	// 1�`���l��1�T���v��������̃T�C�Y(1 or 2)
	short			m_block;	// �u���b�N�T�C�Y(1�T���v��������̑S�`���l�����v�T�C�Y)
	long			m_length;	// �����S�̂̃u���b�N��

	long			m_packet;	// �p�P�b�g�T�C�Y(���̓ǂݏo���œǂݏo�����Ƃ����������T�C�Y(byte) */

	bool			m_ready;
	/*
	 * m_block = m_depth * m_channels;
	 * total size = m_length * m_block
	 * total time = m_length / m_rate
	 */
protected:
	CSLSoundFile(const char * fileName);
public:
	virtual ~CSLSoundFile();

	inline bool is_ready() const { return m_ready; }
	inline int	get_error() const { return m_error; }

	inline int rate() const { return m_rate; }
	inline short channels() const { return m_channels; }
	inline short depth() const { return m_depth; }
	inline long length() const { return m_length; }

	inline short blockSize() const { return m_block; }
	inline long packetSize() const { return m_packet; }
	inline long packetBlocks() const { return m_packet / m_block; }

	virtual long readPacket(void * buf, long blocks = 0) = 0;
	virtual bool seekTop() = 0;
};

class CSLSoundFactory
{
private:
	static CSLSoundFactory	*	m_begin;
	static CSLSoundFactory	*	m_end;

	CSLSoundFactory		*	m_prev;
	CSLSoundFactory		*	m_next;

	const char			*	m_suffix;
	int						m_suffix_length;
	bool					m_ready;

	bool (*m_func_init)();
	void (*m_func_exit)();
	CSLSoundFile * (*m_func_create)(const char * fileName);

public:
	CSLSoundFactory(const char * suffix,
			bool (*func_init)(),
			void (*func_exit)(),
			CSLSoundFile * (*func_create)(const char * fileName));
	~CSLSoundFactory();

	static bool Init();
	static void Exit();
	static CSLSoundFile * Create(const char * fileName);
};

#endif // CSLSoundFile_h
