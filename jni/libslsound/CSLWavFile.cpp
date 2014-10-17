#include "CSLWavFile.h"

// ファクトリの登録
static CSLSoundFactory factory(".wav", 0, 0, CSLWavFile::create);

CSLSoundFile *
CSLWavFile::create(const char * fileName)
{
	return new CSLWavFile(fileName);
}

CSLWavFile::CSLWavFile(const char * fileName)
: CSLSoundFile(fileName)
, m_pcm_top_pos(0)
{
	m_ready = sound_setup();
}

CSLWavFile::~CSLWavFile() {}

long
CSLWavFile::readPacket(void * buf, long blocks)
{
	long size = read(m_fd, buf, blocks * m_block);
	return size / m_block;
}

bool
CSLWavFile::seekTop()
{
	long pos = lseek(m_fd, m_pcm_top_pos, SEEK_SET);
	return (pos == m_pcm_top_pos);
}

short
CSLWavFile::get_short(unsigned char * ptr)
{
	short value = *ptr;
	value |= ((short)*ptr++) << 8;
	return value;
}

long
CSLWavFile::get_long(unsigned char * ptr)
{
	long value = 0;
	for(int i = 0;i < 4; i++) {
		value |= ((long)*(ptr + i)) << (i * 8);
	}
	return value;
}

bool
CSLWavFile::read_chunk(const char * chunkID, long * chunkSize)
{
	unsigned char buf[8];

	*chunkSize = 0;
	int size = read(m_fd, buf, 8);
	if(size != 8) return false;

	int result = strncmp((char *)buf, chunkID, 4);
	*chunkSize = get_long(buf + 4);
	return (!result);
}

bool
CSLWavFile::skip_chunk(const char * chunkID, long * chunkSize)
{
	// chunkID が指定されたもの以外であれば、そのサイズ分を読み飛ばして虫する。
	while(!read_chunk(chunkID, chunkSize)) {
		// chunkのサイズが0の場合は異常なのでエラー
		if(!*chunkSize) return false;
		lseek(m_fd, *chunkSize, SEEK_CUR);
	}
	// 指定されたchunkを見つけたのでリターン

	return true;
}

bool
CSLWavFile::read_fmt()
{
	long chunkSize;
	// 'fmt ' chunk から、PCMフォーマットを取得
	if(!skip_chunk("fmt ", &chunkSize)) return false;

	unsigned char * buf = new unsigned char [ chunkSize ];
	if(!buf) return false;

	if(chunkSize != read(m_fd, buf, chunkSize)) {
		delete [] buf;
		return false;
	}

	short fmt_id = get_short(buf);
	m_channels	= get_short(buf + 2);
	m_rate		= get_long(buf + 4);
	m_block		= get_short(buf + 12);
	m_depth		= get_short(buf + 14) / 8;
	delete [] buf;
	return (fmt_id == 1);	// このクラスではformat type 1(LinearPCM)以外サポートしない
}

bool
CSLWavFile::find_data()
{
	long chunkSize = 0;
	if(!skip_chunk("data", &chunkSize)) return false;

	// 'data' chunk の先頭を記録する
	m_pcm_top_pos = lseek(m_fd, 0L, SEEK_CUR);

	// 'data' chunk のサイズをブロックサイズで割ったものが全体のブロック数
	m_length = chunkSize / m_block;
	return true;
}

bool
CSLWavFile::check_riff()
{
	long bodySize = 0;
	if(!read_chunk("RIFF", &bodySize)) return false;

	// RIFFヘッダの読み込み。
	unsigned char buf[4];
	if(4 != read(m_fd, buf, 4)) return false;
	return !strncmp((char *)buf, "WAVE", 4);
}

bool
CSLWavFile::sound_setup()
{
	if(!check_riff()) return false;
	if(!read_fmt()) return false;
	if(!find_data()) return false;

	m_packet = WAV_PACKET;
	return true;
}
