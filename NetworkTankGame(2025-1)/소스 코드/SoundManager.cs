/*
 * SoundManager(����Ŵ���)
 * ---------------------------------------------------------------
 * ���� �������� BGM(�������)�� SFX(ȿ����)�� ���� �����ϴ� �Ŵ��� Ŭ����
 * 
 * �ֿ� ���:
 * - AudioSource�� AudioClip�� ���� AudioUnit ������ ���带 ������.
 * - Enum(SoundName)�� ���� BGM�� SFX�� �����Ͽ� ���/�ߴ��� �� �ִ�.
 * - Inspector�� ���� �ս��� AudioSource, AudioClip�� ������ �� �ֵ��� ������.
 * - PlayBGM(), PlaySFX(), StopBGM() �޼ҵ� ȣ��� �ٸ� ��ũ��Ʈ���� ��밡��
 * 
 * ���� Ư¡:
 * - AudioSource�� AudioClip�� �̿�ȭ�Ͽ� ������ �ҽ� �߰� �۾� ���� ������ �� �ִ�.
 * - �̸��� �ƴ� Enum ������� ��� ����� �����Ͽ� ��Ÿ�� �����ϰ� �ڵ� �������� ����.
 * - Singleton ������ ����� SoundManager �ν��Ͻ��� �ϳ��� �����ϸ�, ���� �ٲ� �ı����� �ʴ� Ư���� ����.
 * 
 * Ȯ�强:
 * - BGM�� SFX�� �߰��� ������ AudioUnit�� Inspector�� �߰��� �ϸ� �ǹǷ� �ڵ� ���� ���� �߰� ����
 * - �߰� ����� �ɼ� �޴��� ���� ���� ����, ���̵� ��/�ƿ� ��� �� �߰� Ȯ���� �����ϵ��� ������.
 * 
 * ��� ���:
 * 1. SoundManager ������Ʈ�� �̵� �� Audio Units�� ����Ʈ�� �߰��ϰ� SoundName, Audio Source, Audio Clip ���� ����
 * 2. f_PlayBGM() �Ǵ� PlaySFX() �޼��带 ȣ���� Ư�� ���带 �������
 * 3. f_AutoPlayBGM()�� �� �� Start() �޼ҵ� �� ȣ���Ͽ� �ڵ����� ���� �´� ������� �������
 * 4. �ʿ� �� StopBGM()���� Ư�� BGM�� ��������
 * ---------------------------------------------------------------
 */
using NUnit.Framework;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

//Enum"�� "Enumeration"�� ���ڷ�, Ư�� �ڷ����� ���� �̸� ���ǵ� ����� �����Ͽ� ����ϰ��� �� �� ����ϴ� ������ Ÿ��
/// <summary> ���� �� ���� ��� BGM/SFX ��� Enum Ÿ�� </summary>
public enum SoundName
{
    //�������
    BGM_TankScene,   //��ũ �� �������
    BGM_MainMenu,    //���� �޴� �������
    BGM_BeginScene,  //���� ������ �Է� �� �������

    //ȿ����
    SFX_Explosive,      //���� ȿ����     
    SFX_CannonFire,     //��ź �߻� ȿ����
    SFX_AreaSecured,    //�������� Ȯ�� ȿ����
    SFX_AreaLost,       //�������� ��� ȿ����
    SFX_GameOver        //���� ���� ȿ����
}
//[System.Serializable] : C#�� �Ӽ�(attribute) / Ŭ����, ����ü, �ʵ忡 ���밡�� / ����ȭ�� ���� �߰���, �ش� ������ �߰������μ� Inspector�� ǥ���
//Inspector���� ���� ���� �ϱ����� ���� AudioUnit Ŭ���� ����

/// <summary> AudioSource�� AudioClip�� �ϳ��� �������� ���� Ŭ����</summary>
[System.Serializable]
public class AudioUnit
{
    /*
     * C#���� C++�� friend ������ ���⿡, ��ü������ ������ ä�� ȸ���ϴ� ����� ����Ѵ�.
     * Unity���� �����ϴ� ������ [SerializeField] private + public getter ����� ä����
     * ���� AudioUnit Ŭ������ �ʵ�� [SerializeField] private�� ����� �б� ���� public ������Ƽ�� ������
     */
    [SerializeField] private SoundName soundName = 0;         //����� ���� �̸�(Enum���� ����)
    [SerializeField] private AudioSource audioSource = null;  //�Ҹ��� ����ϴ� AudioSource
    [SerializeField] private AudioClip audioClip = null;      //����� ����� ������ AudioClip

    /* �б� ����(Read-Only) ������Ƽ(�ܺο��� �������� ������ �Ұ���)
     * ������Ƽ(Property)�� �Ӽ��̶� �ǹ�.
     * ������Ƽ�� ����ϰ� �Ǹ�, �Ӽ� ���� ��ȯ�ϰų� �� ���� �Ҵ��� �� �ִ�.
     */
    public SoundName SoundName { get { return soundName; } }
    public AudioSource AudioSource { get { return audioSource; } }
    public AudioClip AudioClip { get { return audioClip; } }
}

/// <summary> ���� �������� ���带 �����ϴ� �Ŵ��� Ŭ���� </summary>
public class SoundManager : MonoBehaviour
{
    //singleton pattern: Ŭ���� �ϳ��� �ν��Ͻ��� �ϳ��� �����Ǵ� �����׷��� ����
    private static SoundManager _instance = null;
    public static SoundManager Instance
    {
        get
        {
            if (_instance == null)
            {
                Debug.LogWarning("SoundManager is null.");
            }
            return _instance;
        }
    }

    [Header("Audio Units")] //Inspector���� ����Ʈ�� �����ϱ� ���� �Ӹ���("Audio Units") �߰�
    [SerializeField] private List<AudioUnit> ListUnitBGM = new List<AudioUnit>(); //BGM ���� AudioUnit ����Ʈ
    [SerializeField] private List<AudioUnit> ListUnitSFX = new List<AudioUnit>(); //SFX ���� AudioUnit ����Ʈ

    /// <summary> �� �̸�(Key)�� SoundName(Value)�� 1:1 �����ϴ� ��ųʸ� </summary>
    private Dictionary<string, SoundName> BGMDict = new Dictionary<string, SoundName>()
    {
        //�� ����(Key)�� String, ���� ��Ī(Value)�� ���������� �ۼ��Ǿ� int���̹Ƿ� ��ųʸ��� ����� 1:1 ������
        {"MainMenuScene", SoundName.BGM_MainMenu }, // ���� �޴� �� �������
        {"BeginScene", SoundName.BGM_BeginScene },  // ���� ���� �� �� �������
        {"TankScene", SoundName.BGM_TankScene}      // ��ũ �� �������
    };

    private void Awake()
    {
        if (_instance == null)
        {
            _instance = this;
            DontDestroyOnLoad(gameObject); // InitScene������ ����
        }
        else if (_instance != this)
        {
            Debug.Log("SoundManager has another instance.");
            Destroy(gameObject);
        }
    }

    /// <summary>��Ÿ�� �ʱ�ȭ �Լ� (��� ����, ���� ���� ����)</summary>
    public void f_Init()
    {
        Debug.Log("SoundManager initialized.");
        //TODO: �ʱ� ���� ���� �� ���� ������� �ʱ�ȭ �κ�
    }

    /*
     * foreach ���̶�?
     * ---------------------------------------------------------------
     * �÷���(List, Array ��)�� ����� ��� ��Ҹ� �ϳ��� ������ �ݺ� �����ϴ� �ݺ���
     * 
     * �⺻ ����:
     * foreach (var element in collection)
     * {
     *     // element�� ���� �ݺ������� ������ �۾�
     * }
     * 
     * �۵� ���:
     * - collection(����Ʈ, �迭 ��) �ȿ� �ִ� ��� ��Ҹ� ó������ ������ �ϳ��� �����ϴ�.
     * - ���� ��Ҹ� �ӽ� ����(var element)�� �����Ͽ� ��� �ȿ��� ����մϴ�.
     * - �÷����� ũ�⳪ �ε��� ������ �Ű澲�� �ʾƵ� �˴ϴ�.
     * 
     * ����:
     * - �ڵ尡 ���������ϴ�.
     * - �ε��� ����(��: IndexOutOfRange)�� ������ �� �ֽ��ϴ�.
     * - �б� ���� �۾�(���� �о����)�� �ſ� �����մϴ�.
     * ---------------------------------------------------------------
     * 
     * foreach���� ����ϸ� ����Ʈ ũ�⸸ŭ �ݺ������ϹǷ� �ڵ�ȭ�� ���� �����
     */

    /// <summary> ���� �˸��� ��������� �ڵ����� ����ϴ� �޼ҵ� </summary>
    public void f_AutoPlayBGM()
    {
        SoundName soundName; //��ųʸ����� ��ġ�ϴ� ���ε� ���� �������� ����
        string sSceneName = FlowManager.Instance.f_GetSceneName(); //FlowManager�� ���� ���� �� �̸��� ������

        if (BGMDict.TryGetValue(sSceneName, out soundName))
        {
            f_PlayBGM(soundName, 0.1f); //���ε� ������� 10% �������� ���
        }
        else
        {
            Debug.LogWarning($"BGM mapping not found for scene : {sSceneName}"); //���ε� ���� ���� ��� ��� ���
        }
    }

    /// <summary> �Ű������� ���޹��� ��������� ����ϴ� �޼ҵ� </summary>
    private void f_PlayBGM(SoundName soundName, float volume)
    {
        //���� ����� ������ŭ �ݺ��Ѵٸ� �ڵ�ȭ�� �˻����� 
        foreach (AudioUnit unit in ListUnitBGM) //var��� ��Ȯ���� ���� AudioUnit unit���·� ������
        {
            if (unit.SoundName == soundName) //�Ű����� ���带 ã����?
            {
                unit.AudioSource.clip = unit.AudioClip; //AudioSource�� Clip ����
                unit.AudioSource.volume = volume;       //�Ű������� ���� �������� ����
                unit.AudioSource.loop = true;           //�ݺ� ��� ����
                unit.AudioSource.Play();                //��� ����
                return; //����� ���۵Ǿ����Ƿ� foreach ���� ����
            }
        }
        Debug.LogWarning($"BGM '{soundName}' not found!"); //�Ű������� ���޹��� ��������� ���� ��� ��� ���
    }

    /// <summary> �Ű������� ���޹��� ȿ������ ����ϴ� �޼ҵ� </summary>
    public void f_PlaySFX(SoundName soundName, float volume)
    {
        foreach (AudioUnit unit in ListUnitSFX)
        {
            if (unit.SoundName == soundName)
            {
                unit.AudioSource.volume = volume;               //�Ű������� ���� �������� ����
                unit.AudioSource.PlayOneShot(unit.AudioClip);   //PlayOneShot �޼ҵ带 �̿��Ͽ� 1ȸ�� ȿ���� ���
                return; //����� ���۵Ǿ����Ƿ� foreach ���� ����
            }
        }
        Debug.LogWarning($"SFX '{soundName}' not found!"); //�Ű������� ���޹��� ȿ������ ���� ��� ��� ���
    }

    /// <summary> �Ű������� ���޹��� BGM�� ��� �����ϴ� �޼ҵ� </summary>
    public void f_StopBGM(SoundName soundName)
    {
        foreach (AudioUnit unit in ListUnitBGM)
        {
            if (unit.SoundName == soundName && unit.AudioSource.isPlaying) //������� ���� ��Ī == �Ű����� ���� ��Ī and ���尡 �����
            {
                unit.AudioSource.Stop(); //������� ������� ����
                return; //��� �����Ǿ����Ƿ� foreach ���� ����
            }
        }
    }

    /// <summary> ��� BGM�� ��� �����ϴ� �޼ҵ� </summary>
    public void f_StopAllBGM()
    {
        foreach (AudioUnit unit in ListUnitBGM)
        {
            if (unit.AudioSource.isPlaying)
            {
                unit.AudioSource.Stop();
                return;
            }
        }
    }
}

