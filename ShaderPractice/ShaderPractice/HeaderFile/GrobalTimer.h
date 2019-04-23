#pragma once
typedef uint32_t uint32;
typedef uint64_t uint64;

//Fps固定するクラス
class GrobalTimer : public Singleton<GrobalTimer>
{
private:
	//シングルトン
	friend class Singleton<GrobalTimer>;

	//周波数
	LARGE_INTEGER	m_QPCFrec;
	LARGE_INTEGER	m_QPCLastTime;
	uint64			m_MaxDelta;

	//経過したTick
	uint64			m_ElapsedTick;
	//総Tick数
	uint64			m_TotalTick;
	//最終Tick
	uint64			m_LeftOverTick;

	//フレームレート追跡用
	//フレーム数
	uint32			m_FlameCount;
	//1秒あたりのフレーム
	uint32			m_FlamePerSecond;
	//現フレームの時間
	uint32			m_FlameThisSecond;
	//カウンタ
	uint64			m_QPCSecCouter;

	//フレームレート固定用
	//固定するか
	bool			m_IsFixedStep;
	//fps
	uint64			m_TergetTick;

	GrobalTimer();
	GrobalTimer(const GrobalTimer&);
public:
	~GrobalTimer();

	//一秒あたり10,000,000 Tick
	static const uint64 TickPerSecond = 10000000;

	//Tickと秒変換
	//秒→Tick
	static double				TickToSecond(uint64 Ticks);
	//Tick→秒
	static uint64				SecondToTick(double Second);

	//前のUpdateからの更新時間
	//Tick
	uint64						GetElapsedTick() const;
	//秒
	double						GetElapsedSecond() const;

	//プログラム開始からの経過時間
	//Tick
	uint64						GetTotalTick() const;
	//秒
	double						GetTotalSecond() const;

	//プログラム開始からの合計更新回数
	uint32						GetTotalFrameCount() const;

	//1フレーム当たりの処理時間
	uint32						GetFramePerSecond() const;

	//フレームレート固定するかどうか
	void						SetFixedStepTime(bool IsFixed);

	//フレームレート固定の場合何フレームにするか
	void						SetElapsedTick(uint64 Tick);
	void						SetElapsedSecond(double FramePerSecond);

	//経過時間リセット
	void						ResetElapsedTime();

	//タイマー状態を更新し、Updateを実行
	//Update:更新コールバック関数
	template<typename TickUpdate>
	void Tick(const TickUpdate& Update)
	{
		LARGE_INTEGER CurrentTime;

		//経過時間の計算
		if (!QueryPerformanceCounter(&CurrentTime))
		{

		}
		uint64 TimeDelta = CurrentTime.QuadPart - m_QPCLastTime.QuadPart;

		m_QPCLastTime = CurrentTime;
		m_QPCSecCouter += TimeDelta;

		//最大経過時間を超えたら
		if (TimeDelta > m_MaxDelta)
		{
			TimeDelta = m_MaxDelta;
		}

		TimeDelta *= TickPerSecond;
		TimeDelta /= m_QPCFrec.QuadPart;

		uint32 LastFlame = m_FlameCount;

		//フレームレートを固定するか
		if (m_IsFixedStep)
		{
			if (abs(static_cast<long long int>(TimeDelta - m_TergetTick)) < TickPerSecond / 4000)
			{
				TimeDelta = m_TergetTick;
			}

			m_LeftOverTick += TimeDelta;

			//指定フレームレートになるまで処理待ち
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
			//そのまま更新
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

		//現フレーム情報更新
		if (m_QPCSecCouter >= static_cast<uint64>(m_QPCFrec.QuadPart))
		{
			m_FlamePerSecond = m_FlameThisSecond;
			m_FlameThisSecond = 0;
			m_QPCSecCouter /= m_QPCFrec.QuadPart;
		}
	};
};

