using System;
using System.Collections;
using System.Threading.Tasks;
using UnityEngine;
using TMPro;  // 텍스트 UI를 위한 TextMeshPro
using Unity.Collections;  // Unity Netcode용 FixedString
using Unity.Netcode;  // Unity Netcode for GameObjects 네임스페이스
using UnityEngine.SceneManagement;
using UnityEngine.UI;

// 점수 정보를 네트워크를 통해 전송 가능한 형태로 정의한 구조체
public struct ScoreData : INetworkSerializable
{
    public FixedString64Bytes UserName;  // 사용자 이름
    public int Score;                    // 사용자 점수

    // 네트워크 직렬화 함수
    public void NetworkSerialize<T>(BufferSerializer<T> serializer) where T : IReaderWriter
    {
        serializer.SerializeValue(ref UserName);
        serializer.SerializeValue(ref Score);
    }
}

// 게임 중 UI를 관리하는 싱글턴 매니저 클래스
public class UiManagerTank : MonoBehaviour
{
    // UI 요소 참조들
    public TMP_Text textUserInfo;         // 유저 정보 텍스트
    public TMP_Text textScore;            // 점수판 텍스트
    public TMP_Text textTimer;            // 게임 타이머 텍스트
    public TMP_Text textScoreboardTimer;  // 점수판 타이머 텍스트
    public Slider sliderHealth;           // 체력 표시 슬라이더
    public GameObject readyButton;        // 레디 버튼
    public GameObject rankingPanel;       // 점수판 패널
    public GameObject fadeInImage;        // 페이드인 효과 이미지

    public int health;            // 현재 체력
    public int maxHealth = 100;   // 최대 체력

    private float _playTime;               // 게임 타이머 (카운트다운)
    private float _showScoreboardTime;     // 점수판 표시 시간
    private Color _fadeInImageColor;       // 페이드 이미지의 기본 색

    // 카운트다운 및 점수판 타이머 실행 여부
    private bool ShouldStartCountDown { get; set; }
    private bool ShouldStartScoreboardTimer { get; set; }

    // 싱글턴 인스턴스
    public static UiManagerTank Instance { get; private set; }

    // 객체 초기화 시 싱글턴 패턴 적용
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

    // 게임 시작 시 초기 설정 수행
    private void Start()
    {
        BeginGameManager.Instance.StartNode();         // 네트워크 노드 시작
        BeginGameManager.Instance.UpdateCountId();     // 현재 접속 인원 수 갱신

        InitUI();  // UI 초기화
    }

    // UI 초기화 메서드
    private void InitUI()
    {
        health = maxHealth;  // 체력 초기화

        UpdateTextUserInfo();   // 유저 정보 텍스트 갱신
        UpdateSliderHealth();  // 체력 슬라이더 갱신
        Invoke(nameof(CheckCountId), 2.0f);  // 2초 뒤 접속 인원 수 갱신

        // 타이머 실행 플래그 초기화
        ShouldStartCountDown = false;
        ShouldStartScoreboardTimer = false;

        _playTime = InGameManager.Instance.playTime;  // 총 플레이 타임 설정
        _showScoreboardTime = 10f;  // 점수판 표시 시간

        // UI 초기 상태 세팅
        textTimer.text = InGameManager.Instance.playTime.ToString("F1");
        textScoreboardTimer.text = InGameManager.Instance.playTime.ToString("F1");
        textScore.text = string.Empty;

        rankingPanel.SetActive(false);
        fadeInImage.SetActive(false);
        _fadeInImageColor = fadeInImage.GetComponent<Image>().color;
    }

    // 매 프레임마다 실행되는 메서드
    private void Update()
    {
        if (ShouldStartCountDown)
        {
            UpdateCountDownTimer();  // 게임 타이머 갱신
        }

        if (ShouldStartScoreboardTimer)
        {
            UpdateScoreBoardTimer();  // 점수판 타이머 갱신
        }
    }

    // 게임 타이머 시작
    public void StartTimer()
    {
        ShouldStartCountDown = true;
    }

    // 점수판 타이머 시작
    public void StartScoreboardTimer()
    {
        ShouldStartScoreboardTimer = true;
    }

    // 게임 남은 시간 타이머 감소 및 종료 처리
    private void UpdateCountDownTimer()
    {
        if (!textTimer) return;

        _playTime -= Time.deltaTime;
        textTimer.text = _playTime.ToString("F1");

        if (!(_playTime <= 0)) return;

        ShouldStartCountDown = false;

        InGameManager.Instance.StartScoreboardTimer();  // 점수판 타이머 시작
        rankingPanel.SetActive(true);                   // 점수판 표시
        InGameManager.Instance.CollectScoresServerRpc(); // 서버에서 점수 수집 요청
    }

    // 점수판 남은 시간 타이머 감소 및 페이드인 시작
    private void UpdateScoreBoardTimer()
    {
        if (!textScoreboardTimer) return;

        InGameManager.Instance.CanMove = false;  // 캐릭터 이동 금지

        _showScoreboardTime -= Time.deltaTime;
        textScoreboardTimer.text = _showScoreboardTime.ToString("F1");

        if (!(_showScoreboardTime <= 0f)) return;

        ShouldStartScoreboardTimer = false;
        StartFadeIn();  // 페이드인 효과 시작
    }

    // 외부에서 체력 업데이트 요청 시 호출
    public void UpdateUIInfo(int value)
    {
        health = value;
        UpdateTextUserInfo();
        UpdateSliderHealth();
    }

    // 유저 정보 텍스트 갱신 (인원 수 : 사용자 ID = 체력)
    private void UpdateTextUserInfo()
    {
        textUserInfo.text = $"{BeginGameManager.Instance.CountId}:{BeginGameManager.Instance.UserId}={health}";
    }

    // 점수판 UI 갱신
    public void UpdateScoreUI(ScoreData[] scoreData)
    {
        textScore.text = "";

        if (scoreData.Length == 0)
        {
            textScore.text = "Score not recorded";
            return;
        }

        // 모든 유저 점수 출력
        foreach (var data in scoreData)
        {
            textScore.text += $"{data.UserName} : {data.Score}\n";
        }
    }

    // 체력 슬라이더 갱신
    private void UpdateSliderHealth()
    {
        sliderHealth.value = health;
    }

    // BeginGameManager에서 인원 수 재확인 후 UI 반영
    private void CheckCountId()
    {
        BeginGameManager.Instance.UpdateCountId();
        UpdateTextUserInfo();
    }

    // 페이드인 효과 코루틴
    private IEnumerator FadeIn()
    {
        float alphaValue = 0f;
        fadeInImage.GetComponent<Image>().color = new Color(_fadeInImageColor.r, _fadeInImageColor.g, _fadeInImageColor.b, alphaValue);
        fadeInImage.SetActive(true);

        while (true)
        {
            yield return new WaitForSeconds(0.1f);
            alphaValue += 0.1f;
            fadeInImage.GetComponent<Image>().color = new Color(_fadeInImageColor.r, _fadeInImageColor.g, _fadeInImageColor.b, alphaValue);

            if (alphaValue >= 1f)
            {
                break;
            }
        }

        EndGameSequence();  // 페이드 완료 후 엔딩 처리
    }

    // 페이드인 효과 시작
    private void StartFadeIn()
    {
        StartCoroutine(nameof(FadeIn));
    }

    // 게임 종료 처리 로직
    private async void EndGameSequence()
    {
        try
        {
            InGameManager.Instance.RequestDestroyAllClients();  // 클라이언트 제거 요청
            await Task.Delay(100);  // 잠깐 대기
            LoadEndScene();         // 종료 씬 로드
        }
        catch (Exception e)
        {
            Debug.LogError($"Error : {e.Message}");
        }
    }

    // 종료 씬 로딩 (서버/클라 분기)
    public void LoadEndScene()
    {
        if (NetworkManager.Singleton.IsServer)
        {
            SoundManager.Instance.StopPlaySound(SoundType.ThemeTank);  // 배경음 정지
            NetworkManager.Singleton.SceneManager.LoadScene("EndScene", LoadSceneMode.Single);  // 서버에서 씬 로드
            return;
        }

        SceneManager.LoadScene("EndScene");  // 클라이언트에서 씬 로드
    }

    // 레디 버튼 클릭 이벤트 처리
    public void OnReadyButtonClicked()
    {
        PlayerReadyState.Instance.SubmitReadyServerRpc();  // 서버에 준비 완료 전달
        readyButton.SetActive(false);  // 버튼 비활성화
    }
}
