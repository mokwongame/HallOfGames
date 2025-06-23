using TMPro;                       // TextMeshPro 입력 필드 사용
using UnityEngine;
using UnityEngine.SceneManagement; // 씬 전환을 위한 네임스페이스
using UnityEngine.UI;             // UI 버튼 제어용

// 게임 시작(로비) 화면에서 UI를 제어하는 클래스
public class UiManagerBegin : MonoBehaviour
{
    // 사용자 입력 UI 요소들
    public TMP_InputField inputIp;         // IP 주소 입력 필드
    public TMP_InputField inputPort;       // 포트 입력 필드
    public TMP_InputField inputUserId;     // 사용자 ID 입력 필드

    // 시작 화면 패널과 버튼들
    public GameObject startPanel;          // 호스트/클라이언트 선택 패널
    public GameObject startBtn;            // 시작 버튼
    public GameObject exitBtn;             // 종료 버튼

    // 기본 설정값 상수
    private const string DefaultIpAddress = "127.0.0.1";  // 로컬호스트 기본 IP
    private const string DefaultUserName = "player";      // 기본 사용자 이름
    private const string DefaultPort = "7777";            // 기본 포트

    // 싱글턴 인스턴스
    public static UiManagerBegin Instance { get; private set; }

    // 객체 생성 시 싱글턴 패턴 적용
    private void Awake()
    {
        if (!Instance)
        {
            Instance = this;
        }
        else if (Instance != this)
        {
            Destroy(gameObject);
        }
    }

    // 씬 시작 시 초기 UI 세팅
    private void Start()
    {
        startPanel.SetActive(false);  // 시작 패널 숨김
        SoundManager.Instance.PlaySound(SoundType.ThemeBegin, true);  // 시작 테마 음악 반복 재생
    }

    // 호스트로 게임 시작
    public void StartHost()
    {
        BeginGameManager.Instance.UserNodeType = BeginGameManager.NodeType.Host;  // 사용자 노드 타입 설정
        UpdateConnection();     // IP, 포트, 사용자 ID 정보 설정
        LoadTankScene();        // 메인 게임 씬 로드
    }

    // 클라이언트로 게임 시작
    public void StartClient()
    {
        BeginGameManager.Instance.UserNodeType = BeginGameManager.NodeType.Client;
        UpdateConnection();
        LoadTankScene();
    }

    // 서버로 게임 시작 (GUI에서 사용할 경우 제한적)
    public void StartServer()
    {
        BeginGameManager.Instance.UserNodeType = BeginGameManager.NodeType.Server;
        UpdateConnection();
        LoadTankScene();
    }

    // 게임 본편(탱크 씬) 로딩
    private void LoadTankScene()
    {
        SoundManager.Instance.StopPlaySound(SoundType.ThemeBegin);  // 음악 정지
        SceneManager.LoadScene("TankScene");                        // 씬 전환
    }

    // IP, 포트, 사용자 이름을 BeginGameManager에 전달
    private void UpdateConnection()
    {
        // 입력값이 없으면 기본값 사용
        if (string.IsNullOrEmpty(inputIp.text))
        {
            inputIp.text = DefaultIpAddress;
        }

        if (string.IsNullOrEmpty(inputPort.text))
        {
            inputPort.text = DefaultPort;
        }

        if (string.IsNullOrEmpty(inputUserId.text))
        {
            inputUserId.text = DefaultUserName;
        }

        // 문자열로 입력받은 포트를 ushort로 변환
        ushort portNum = ushort.Parse(inputPort.text);

        // BeginGameManager에 연결 정보 등록
        BeginGameManager.Instance.SetConnection(inputIp.text, portNum);
        BeginGameManager.Instance.UserId = inputUserId.text;
    }

    // '게임 시작' 버튼 클릭 시 호출
    public void OnStartBtnClicked()
    {
        startPanel.SetActive(true);  // 호스트/클라이언트 선택 창 표시

        // 중복 클릭 방지를 위해 버튼 비활성화
        startBtn.GetComponent<Button>().interactable = false;
        exitBtn.GetComponent<Button>().interactable = false;
    }

    // '종료' 버튼 클릭 시 호출
    public void OnExitBtnClicked()
    {
        SoundManager.Instance.StopPlaySound(SoundType.ThemeBegin);      // 음악 정지

#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;                // 에디터에서 정지
#else 
        Application.Quit();  // 빌드된 앱 종료
#endif
    }

    // '닫기' 버튼 클릭 시 시작 패널 숨기고 버튼 다시 활성화
    public void OnHidePanelBtnClicked()
    {
        startPanel.SetActive(false);

        startBtn.GetComponent<Button>().interactable = true;
        exitBtn.GetComponent<Button>().interactable = true;
    }
}
