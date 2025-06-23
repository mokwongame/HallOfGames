using Unity.Netcode;  // Unity Netcode for GameObjects 네트워크 기능을 위한 네임스페이스
using Unity.Netcode.Transports.UTP;  // UTP(Unity Transport Protocol) 사용을 위한 네임스페이스
using UnityEngine;  // Unity 엔진 기본 기능 사용

// 게임 시작 시 네트워크 설정 및 초기 파라미터를 관리하는 매니저 클래스
public class BeginGameManager : MonoBehaviour
{
    // 노드 유형을 정의하는 열거형: 호스트, 클라이언트, 서버
    public enum NodeType
    {
        Null = 0,  // 초기 미지정 상태
        Host,      // 호스트 (서버 + 클라이언트 역할)
        Client,    // 클라이언트
        Server     // 전용 서버
    }

    // 현재 사용자 노드 유형 (프로퍼티)
    public NodeType UserNodeType
    {
        get;
        set;
    }

    // 사용자 ID (외부에서 get/set 가능)
    public string UserId
    {
        get;
        set;
    }

    // 접속한 클라이언트 수 (읽기 전용)
    public int CountId
    {
        get;
        private set;
    }

    // 탱크 이동 속도 (읽기 전용)
    public float SpeedTank
    {
        get;
        private set;
    }

    // 탱크 회전 속도 (읽기 전용)
    public float RotSpeedTank
    {
        get;
        private set;
    }

    // 총알 속도 (읽기 전용)
    public float BulletSpeed
    {
        get;
        private set;
    }

    // 고정 프레임 설정값 (60fps)
    private const int TargetFrameRate = 60;

    // BeginGameManager 싱글턴 인스턴스
    public static BeginGameManager Instance { get; private set; }

    // 게임 시작 시 호출되는 Unity 기본 메서드
    private void Start()
    {
        Application.targetFrameRate = TargetFrameRate;  // 게임 프레임 고정
        InitParam();  // 초기 값 설정
    }

    // 오브젝트가 처음 생성될 때 실행되는 메서드
    private void Awake()
    {
        // 싱글턴 패턴: 인스턴스가 없으면 현재 인스턴스로 설정
        if (!Instance)
        {
            Instance = this;
        }
        // 이미 다른 인스턴스가 존재하면 파괴
        else if (Instance != this)
        {
            Destroy(gameObject);
        }

        // 씬 전환 시에도 이 오브젝트가 파괴되지 않도록 설정
        DontDestroyOnLoad(gameObject);
    }

    // 기본 파라미터 값 초기화
    private void InitParam()
    {
        UserNodeType = NodeType.Null;  // 노드 타입 초기화
        SpeedTank = 10.0f;             // 탱크 속도 초기화
        RotSpeedTank = 50.0f;          // 회전 속도 초기화
        BulletSpeed = 10000f;          // 총알 속도 초기화
    }

    // 현재 연결된 클라이언트 수를 가져오는 메서드
    private int GetNumClients()
    {
        // 네트워크 매니저가 존재하지 않으면 0 반환
        if (!NetworkManager.Singleton)
        {
            return 0;
        }

        // 연결된 클라이언트 리스트 가져오기
        System.Collections.Generic.IReadOnlyList<NetworkClient> connectedClients = NetworkManager.Singleton.ConnectedClientsList;
        return connectedClients.Count;
    }

    // IP주소와 포트 번호를 통해 네트워크 연결 설정
    public void SetConnection(string ipAddress, ushort portNum)
    {
        NetworkManager.Singleton.GetComponent<UnityTransport>().SetConnectionData(ipAddress, portNum);
    }

    // 호스트(서버+클라이언트)를 시작하는 메서드
    private void StartHost()
    {
        // 이미 서버나 클라이언트로 실행 중이 아니라면 호스트 시작
        if (!NetworkManager.Singleton.IsServer && !NetworkManager.Singleton.IsClient)
        {
            NetworkManager.Singleton.StartHost();
        }
    }

    // 전용 서버를 시작하는 메서드
    private void StartServer()
    {
        // 이미 서버나 클라이언트로 실행 중이 아니라면 서버 시작
        if (!NetworkManager.Singleton.IsServer && !NetworkManager.Singleton.IsClient)
        {
            NetworkManager.Singleton.StartServer();
        }
    }

    // 클라이언트를 시작하는 메서드
    private void StartClient()
    {
        // 이미 서버나 클라이언트로 실행 중이 아니라면 클라이언트 시작
        if (!NetworkManager.Singleton.IsServer && !NetworkManager.Singleton.IsClient)
        {
            NetworkManager.Singleton.StartClient();
        }
    }

    // UserNodeType 값을 기반으로 호스트/클라이언트/서버 중 적절한 노드를 시작
    public void StartNode()
    {
        if (UserNodeType == NodeType.Host)
        {
            StartHost();
        }
        else if (UserNodeType == NodeType.Client)
        {
            StartClient();
        }
        else if (UserNodeType == NodeType.Server)
        {
            StartServer();
        }
    }

    // CountId 값을 현재 연결된 클라이언트 수로 갱신
    public void UpdateCountId()
    {
        CountId = GetNumClients();
    }
}
