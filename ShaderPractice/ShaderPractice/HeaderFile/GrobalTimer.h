#pragma once
typedef uint32_t uint32;
typedef uint64_t uint64;

//Fps�Œ肷��N���X
class GrobalTimer : public Singleton<GrobalTimer>
{
private:
	//�V���O���g��
	friend class Singleton<GrobalTimer>;

	//���g��
	LARGE_INTEGER	m_QPCFrec;
	LARGE_INTEGER	m_QPCLastTime;
	uint64			m_MaxDelta;

	//�o�߂���Tick
	uint64			m_ElapsedTick;
	//��Tick��
	uint64			m_TotalTick;
	//�ŏITick
	uint64			m_LeftOverTick;

	//�t���[�����[�g�ǐ՗p
	//�t���[����
	uint32			m_FlameCount;
	//1�b������̃t���[��
	uint32			m_FlamePerSecond;
	//���t���[���̎���
	uint32			m_FlameThisSecond;
	//�J�E���^
	uint64			m_QPCSecCouter;

	//�t���[�����[�g�Œ�p
	//�Œ肷�邩
	bool			m_IsFixedStep;
	//fps
	uint64			m_TergetTick;

	GrobalTimer();
	GrobalTimer(const GrobalTimer&);
public:
	~GrobalTimer();

	//��b������10,000,000 Tick
	static const uint64 TickPerSecond = 10000000;

	//Tick�ƕb�ϊ�
	//�b��Tick
	static double				TickToSecond(uint64 Ticks);
	//Tick���b
	static uint64				SecondToTick(double Second);

	//�O��Update����̍X�V����
	//Tick
	uint64						GetElapsedTick() const;
	//�b
	double						GetElapsedSecond() const;

	//�v���O�����J�n����̌o�ߎ���
	//Tick
	uint64						GetTotalTick() const;
	//�b
	double						GetTotalSecond() const;

	//�v���O�����J�n����̍��v�X�V��
	uint32						GetTotalFrameCount() const;

	//1�t���[��������̏�������
	uint32						GetFramePerSecond() const;

	//�t���[�����[�g�Œ肷�邩�ǂ���
	void						SetFixedStepTime(bool IsFixed);

	//�t���[�����[�g�Œ�̏ꍇ���t���[���ɂ��邩
	void						SetElapsedTick(uint64 Tick);
	void						SetElapsedSecond(double FramePerSecond);

	//�o�ߎ��ԃ��Z�b�g
	void						ResetElapsedTime();

	//�^�C�}�[��Ԃ��X�V���AUpdate�����s
	//Update:�X�V�R�[���o�b�N�֐�
	template<typename TickUpdate>
	void Tick(const TickUpdate& Update)
	{
		LARGE_INTEGER CurrentTime;

		//�o�ߎ��Ԃ̌v�Z
		if (!QueryPerformanceCounter(&CurrentTime))
		{

		}
		uint64 TimeDelta = CurrentTime.QuadPart - m_QPCLastTime.QuadPart;

		m_QPCLastTime = CurrentTime;
		m_QPCSecCouter += TimeDelta;

		//�ő�o�ߎ��Ԃ𒴂�����
		if (TimeDelta > m_MaxDelta)
		{
			TimeDelta = m_MaxDelta;
		}

		TimeDelta *= TickPerSecond;
		TimeDelta /= m_QPCFrec.QuadPart;

		uint32 LastFlame = m_FlameCount;

		//�t���[�����[�g���Œ肷�邩
		if (m_IsFixedStep)
		{
			if (abs(static_cast<long long int>(TimeDelta - m_TergetTick)) < TickPerSecond / 4000)
			{
				TimeDelta = m_TergetTick;
			}

			m_LeftOverTick += TimeDelta;

			//�w��t���[�����[�g�ɂȂ�܂ŏ����҂�
			while (m_LeftOverTick >= m_TergetTick)
			{
				m_ElapsedTick = m_TergetTick;
				m_TotalTick += m_TergetTick;
				m_LeftOverTick -= m_TergetTick;
				m_FlameCount++;

				Update();
			}
		}
		else
		{
			//���̂܂܍X�V
			m_ElapsedTick = TimeDelta;
			m_TotalTick += TimeDelta;
			m_LeftOverTick = 0;
			m_FlameCount++;

			Update();
		}

		if (m_FlameCount != LastFlame)
		{
			m_FlameThisSecond++;
		}

		//���t���[�����X�V
		if (m_QPCSecCouter >= static_cast<uint64>(m_QPCFrec.QuadPart))
		{
			m_FlamePerSecond = m_FlameThisSecond;
			m_FlameThisSecond = 0;
			m_QPCSecCouter /= m_QPCFrec.QuadPart;
		}
	};
};

