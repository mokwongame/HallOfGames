/*
 * [FlowManager(�÷ο� �Ŵ���)]
 * ������ �� ��ȯ �� ��ü �帧�� �����ϴ� �̱��� �Ŵ��� Ŭ����
 * - �� ��ȯ(Scene ��ȯ)
 * - ���� �� �̸� ��ȸ
 */

using UnityEngine;
using UnityEngine.SceneManagement;

/// <summary> ������Ʈ ������ ����� �� �̸� ������(Enum) </summary>
public enum SceneName
{
    MainMenuScene,  //���� �޴� ��
    BeginScene,     //���� ������ �Է� ��
    TankScene,      //���� ��
    EndScene        //���� ���� ��
}

/// <summary> ������ �帧�� �����ϴ� FlowManager Ŭ���� </summary>
public class FlowManager : MonoBehaviour
{
    private static FlowManager _instance = null;

    public static FlowManager Instance
    {
        get
        {
            if (_instance == null)
            {
                Debug.LogWarning("FlowManager is null.");
            }
            return _instance;
        }
    }

    private void Awake()
    {
        if (_instance == null)
        {
            _instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else if (_instance != this)
        {
            Debug.Log("FlowManager has another instance.");
            Destroy(gameObject);
        }
    }

    void Start()
    {

    }


    /// <summary> ������ ������ ��ȯ�ϴ� �޼ҵ� </summary>
    public void f_OpenScene(SceneName sceneName)
    {
        SceneManager.LoadScene(sceneName.ToString());
    }

    /// <summary> ���� ���� SceneName Enum���� ��ȯ�ϴ� �޼ҵ� </summary>
    public SceneName f_GetCurrentSceneName()
    {
        string sceneName = SceneManager.GetActiveScene().name;

        if (System.Enum.TryParse(sceneName, out SceneName currentScene))
        {
            return currentScene;
        }
        else
        {
            Debug.LogWarning($"�� {sceneName}�� SceneName Enum�� �������� �ʽ��ϴ�.");
            return SceneName.BeginScene; //���� ��Ȳ �߻� �� �⺻ �� ��ȯ
        }
    }

    /// <summary> ���� �� �̸��� ���ڿ��� ��ȯ�ϴ� �޼ҵ� (���� ���� �� �ܺ� Ȱ���) </summary>
    public string f_GetSceneName()
    {
        return SceneManager.GetActiveScene().name;
    }
}
