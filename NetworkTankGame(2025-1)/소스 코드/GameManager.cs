using Unity.Netcode;
using Unity.Netcode.Transports.UTP;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public enum NodeType
    { NULL = 0, HOST, CLIENT, SERVER }

    public NodeType UserNodeType
    { get; set; }

    public string UserId // 클라이언트 이름
    { get; set; }

    public int CountId // 클라이언트의 등장 순서
    { get; set; }

    public float SpeedTank
    { get; set; }

    public float RotSpeedTank
    { get; set; }

    public float ForceBomb // F = m*a
    { get; set; }

    private static GameManager _instance = null;
    public static GameManager Instance
    {
        get
        {
            if (_instance == null)
            {
                Debug.Log("GameManager is null.");
            }
            return _instance;
        }
    }

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        initParam();
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
            Debug.Log("GameManager has another instance.");
            Destroy(gameObject);
        }
        DontDestroyOnLoad(gameObject);
    }

    void initParam()
    {
        UserNodeType = NodeType.NULL;
        SpeedTank = 6.0f;
        RotSpeedTank = 50.0f;
        ForceBomb = 2000.0f;
    }

    public int getNumClients()
    {
        if (NetworkManager.Singleton == null) return 0;
        System.Collections.Generic.IReadOnlyList<NetworkClient> connectedClients = NetworkManager.Singleton.ConnectedClientsList;
        return connectedClients.Count;
    }

    public void setConnection(string ipAddress, ushort portNum)
    {
        NetworkManager.Singleton.GetComponent<UnityTransport>().SetConnectionData(ipAddress, portNum);
    }

    public void startHost()
    {
        if (!NetworkManager.Singleton.IsServer && !NetworkManager.Singleton.IsClient)
        {
            Debug.Log("Host started.");
            NetworkManager.Singleton.StartHost();
        }
    }

    public void startServer()
    {
        if (!NetworkManager.Singleton.IsServer && !NetworkManager.Singleton.IsClient)
        {
            Debug.Log("Server started.");
            NetworkManager.Singleton.StartServer();
        }
    }

    public void startClient()
    {
        if (!NetworkManager.Singleton.IsServer && !NetworkManager.Singleton.IsClient)
        {
            Debug.Log("Client started.");
            NetworkManager.Singleton.StartClient();
        }
    }

    public void startNode()
    {
        if (UserNodeType == NodeType.HOST)
            startHost();
        else if (UserNodeType == NodeType.CLIENT)
            startClient();
        else if (UserNodeType == NodeType.SERVER)
            startServer();
        else
        {
            Debug.Log("Unknown node type.");
        }
    }

    public void updateCountId()
    {
        CountId = getNumClients();
    }

    //점령 시스템에 사용되는 모든 플레이어에게 데미지를 적용하는 메소드
    public void f_DamageAllExcept(GameObject excluding, int nDamage)
    {
        foreach (NetworkClient client in NetworkManager.Singleton.ConnectedClientsList) //모든 클라이언트에 대해 반복
        {
            GameObject gClient = client.PlayerObject.gameObject; //클라이언트의 PlayerObject를 가져옴
            if (gClient != excluding) //제외할 플레이어(점령자)가 아닌 경우
            {
                PlayerHealthNet health = gClient.GetComponent<PlayerHealthNet>(); //PlayerHealthNet 컴포넌트를 가져옴
                if (health != null)
                {
                    health.decHealthByAmountRpc(nDamage); //플레이어의 체력을 감소시키는 RPC 호출
                }
                SoundManager.Instance.f_PlaySFX(SoundName.SFX_AreaLost, 0.7f); //점령 실패 효과음 재생
            }
        }
    }


}
