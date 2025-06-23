using TMPro;
using UnityEngine;
using UnityEngine.InputSystem.Users;
using UnityEngine.SceneManagement;
using UnityEngine.Windows;

public class UiManagerBegin : MonoBehaviour
{
    public TMP_InputField inputIp;
    public TMP_InputField inputPort;
    public TMP_InputField inputUserId;

    private static UiManagerBegin _instance = null;
    public static UiManagerBegin Instance
    {
        get
        {
            if (_instance == null)
            {
                Debug.Log("UiManagerBegin is null.");
            }
            return _instance;
        }
    }

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        SoundManager.Instance.f_StopAllBGM();  //모든 배경음악 정지
        SoundManager.Instance.f_AutoPlayBGM(); //Begin 씬에서 자동으로 배경음악 재생
    }

    // Update is called once per frame
    void Update()
    {

    }

    private void Awake()
    {
        if (_instance == null)
        {
            _instance = this;
        }
        else if (_instance != this)
        {
            Debug.Log("UiManagerBegin has another instance.");
            Destroy(gameObject);
        }
    }

    public void startHost()
    {
        GameManager.Instance.UserNodeType = GameManager.NodeType.HOST;
        updateConnection();
        FlowManager.Instance.f_OpenScene(SceneName.TankScene); //FlowManager를 통해 TankScene씬 전환
    }

    public void startClient()
    {
        GameManager.Instance.UserNodeType = GameManager.NodeType.CLIENT;
        updateConnection();
        FlowManager.Instance.f_OpenScene(SceneName.TankScene); //FlowManager를 통해 TankScene씬 전환
    }

    public void startServer()
    {
        GameManager.Instance.UserNodeType = GameManager.NodeType.SERVER;
        updateConnection();
        FlowManager.Instance.f_OpenScene(SceneName.TankScene); //FlowManager를 통해 TankScene씬 전환
    }

    public void updateConnection()
    {
        string ipAddress = inputIp.text;
        if (string.IsNullOrEmpty(ipAddress))
        {
            ipAddress = "127.0.0.1";
        }
        string port = inputPort.text;
        if (string.IsNullOrEmpty(port))
        {
            port = "7777";
        }
        string userId = inputUserId.text;
        if (string.IsNullOrEmpty(userId))
        {
            userId = "player";
        }
        ushort portNum = ushort.Parse(port);
        GameManager.Instance.setConnection(ipAddress, portNum);
        GameManager.Instance.UserId = userId;
    }
}
