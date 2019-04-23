#pragma once

//2�ȏ�̓����I�u�W�F�N�g�𑶍݂����Ȃ��e���v���[�g
template <class T> class Singleton
{
private:
	typedef std::unique_ptr<T> singleton_pointer_type;
	//�C���X�^���X�쐬
	inline static T *CreateInstance() { return new T(); }
	inline static T &GetReference(const singleton_pointer_type &ptr) { return *ptr; }

	//�R�s�[�֎~
	Singleton(const Singleton &) = delete;
	Singleton& operator=(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton& operator=(Singleton &&) = delete;
protected:
	Singleton() {}
	~Singleton() {}
public:
	//�ŏ��ɌĂ΂ꂽ���̂݃C���X�^���X�����
	static T& GetInstance()
	{
		static typename T::singleton_pointer_type s_Singleton(T::CreateInstance());

		return GetReference(s_Singleton);
	}
};