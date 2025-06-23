using System;
using System.Collections;
using System.Threading.Tasks;
using UnityEngine;
using TMPro;  // �ؽ�Ʈ UI�� ���� TextMeshPro
using Unity.Collections;  // Unity Netcode�� FixedString
using Unity.Netcode;  // Unity Netcode for GameObjects ���ӽ����̽�
using UnityEngine.SceneManagement;
using UnityEngine.UI;

// ���� ������ ��Ʈ��ũ�� ���� ���� ������ ���·� ������ ����ü
public struct ScoreData : INetworkSerializable
{
    public FixedString64Bytes UserName;  // ����� �̸�
    public int Score;                    // ����� ����

    // ��Ʈ��ũ ����ȭ �Լ�
    public void NetworkSerialize<T>(BufferSerializer<T> serializer) where T : IReaderWriter
    {
        serializer.SerializeValue(ref UserName);
        serializer.SerializeValue(ref Score);
    }
}

// ���� �� UI�� �����ϴ� �̱��� �Ŵ��� Ŭ����
public class UiManagerTank : MonoBehaviour
{
    // UI ��� ������
    public TMP_Text textUserInfo;         // ���� ���� �ؽ�Ʈ
    public TMP_Text textScore;            // ������ �ؽ�Ʈ
    public TMP_Text textTimer;            // ���� Ÿ�̸� �ؽ�Ʈ
    public TMP_Text textScoreboardTimer;  // ������ Ÿ�̸� �ؽ�Ʈ
    public Slider sliderHealth;           // ü�� ǥ�� �����̴�
    public GameObject readyButton;        // ���� ��ư
    public GameObject rankingPanel;       // ������ �г�
    public GameObject fadeInImage;        // ���̵��� ȿ�� �̹���

    public int health;            // ���� ü��
    public int maxHealth = 100;   // �ִ� ü��

    private float _playTime;               // ���� Ÿ�̸� (ī��Ʈ�ٿ�)
    private float _showScoreboardTime;     // ������ ǥ�� �ð�
    private Color _fadeInImageColor;       // ���̵� �̹����� �⺻ ��

    // ī��Ʈ�ٿ� �� ������ Ÿ�̸� ���� ����
    private bool ShouldStartCountDown { get; set; }
    private bool ShouldStartScoreboardTimer { get; set; }

    // �̱��� �ν��Ͻ�
    public static UiManagerTank Instance { get; private set; }

    // ��ü �ʱ�ȭ �� �̱��� ���� ����
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

    // ���� ���� �� �ʱ� ���� ����
    private void Start()
    {
        BeginGameManager.Instance.StartNode();         // ��Ʈ��ũ ��� ����
        BeginGameManager.Instance.UpdateCountId();     // ���� ���� �ο� �� ����

        InitUI();  // UI �ʱ�ȭ
    }

    // UI �ʱ�ȭ �޼���
    private void InitUI()
    {
        health = maxHealth;  // ü�� �ʱ�ȭ

        UpdateTextUserInfo();   // ���� ���� �ؽ�Ʈ ����
        UpdateSliderHealth();  // ü�� �����̴� ����
        Invoke(nameof(CheckCountId), 2.0f);  // 2�� �� ���� �ο� �� ����

        // Ÿ�̸� ���� �÷��� �ʱ�ȭ
        ShouldStartCountDown = false;
        ShouldStartScoreboardTimer = false;

        _playTime = InGameManager.Instance.playTime;  // �� �÷��� Ÿ�� ����
        _showScoreboardTime = 10f;  // ������ ǥ�� �ð�

        // UI �ʱ� ���� ����
        textTimer.text = InGameManager.Instance.playTime.ToString("F1");
        textScoreboardTimer.text = InGameManager.Instance.playTime.ToString("F1");
        textScore.text = string.Empty;

        rankingPanel.SetActive(false);
        fadeInImage.SetActive(false);
        _fadeInImageColor = fadeInImage.GetComponent<Image>().color;
    }

    // �� �����Ӹ��� ����Ǵ� �޼���
    private void Update()
    {
        if (ShouldStartCountDown)
        {
            UpdateCountDownTimer();  // ���� Ÿ�̸� ����
        }

        if (ShouldStartScoreboardTimer)
        {
            UpdateScoreBoardTimer();  // ������ Ÿ�̸� ����
        }
    }

    // ���� Ÿ�̸� ����
    public void StartTimer()
    {
        ShouldStartCountDown = true;
    }

    // ������ Ÿ�̸� ����
    public void StartScoreboardTimer()
    {
        ShouldStartScoreboardTimer = true;
    }

    // ���� ���� �ð� Ÿ�̸� ���� �� ���� ó��
    private void UpdateCountDownTimer()
    {
        if (!textTimer) return;

        _playTime -= Time.deltaTime;
        textTimer.text = _playTime.ToString("F1");

        if (!(_playTime <= 0)) return;

        ShouldStartCountDown = false;

        InGameManager.Instance.StartScoreboardTimer();  // ������ Ÿ�̸� ����
        rankingPanel.SetActive(true);                   // ������ ǥ��
        InGameManager.Instance.CollectScoresServerRpc(); // �������� ���� ���� ��û
    }

    // ������ ���� �ð� Ÿ�̸� ���� �� ���̵��� ����
    private void UpdateScoreBoardTimer()
    {
        if (!textScoreboardTimer) return;

        InGameManager.Instance.CanMove = false;  // ĳ���� �̵� ����

        _showScoreboardTime -= Time.deltaTime;
        textScoreboardTimer.text = _showScoreboardTime.ToString("F1");

        if (!(_showScoreboardTime <= 0f)) return;

        ShouldStartScoreboardTimer = false;
        StartFadeIn();  // ���̵��� ȿ�� ����
    }

    // �ܺο��� ü�� ������Ʈ ��û �� ȣ��
    public void UpdateUIInfo(int value)
    {
        health = value;
        UpdateTextUserInfo();
        UpdateSliderHealth();
    }

    // ���� ���� �ؽ�Ʈ ���� (�ο� �� : ����� ID = ü��)
    private void UpdateTextUserInfo()
    {
        textUserInfo.text = $"{BeginGameManager.Instance.CountId}:{BeginGameManager.Instance.UserId}={health}";
    }

    // ������ UI ����
    public void UpdateScoreUI(ScoreData[] scoreData)
    {
        textScore.text = "";

        if (scoreData.Length == 0)
        {
            textScore.text = "Score not recorded";
            return;
        }

        // ��� ���� ���� ���
        foreach (var data in scoreData)
        {
            textScore.text += $"{data.UserName} : {data.Score}\n";
        }
    }

    // ü�� �����̴� ����
    private void UpdateSliderHealth()
    {
        sliderHealth.value = health;
    }

    // BeginGameManager���� �ο� �� ��Ȯ�� �� UI �ݿ�
    private void CheckCountId()
    {
        BeginGameManager.Instance.UpdateCountId();
        UpdateTextUserInfo();
    }

    // ���̵��� ȿ�� �ڷ�ƾ
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

        EndGameSequence();  // ���̵� �Ϸ� �� ���� ó��
    }

    // ���̵��� ȿ�� ����
    private void StartFadeIn()
    {
        StartCoroutine(nameof(FadeIn));
    }

    // ���� ���� ó�� ����
    private async void EndGameSequence()
    {
        try
        {
            InGameManager.Instance.RequestDestroyAllClients();  // Ŭ���̾�Ʈ ���� ��û
            await Task.Delay(100);  // ��� ���
            LoadEndScene();         // ���� �� �ε�
        }
        catch (Exception e)
        {
            Debug.LogError($"Error : {e.Message}");
        }
    }

    // ���� �� �ε� (����/Ŭ�� �б�)
    public void LoadEndScene()
    {
        if (NetworkManager.Singleton.IsServer)
        {
            SoundManager.Instance.StopPlaySound(SoundType.ThemeTank);  // ����� ����
            NetworkManager.Singleton.SceneManager.LoadScene("EndScene", LoadSceneMode.Single);  // �������� �� �ε�
            return;
        }

        SceneManager.LoadScene("EndScene");  // Ŭ���̾�Ʈ���� �� �ε�
    }

    // ���� ��ư Ŭ�� �̺�Ʈ ó��
    public void OnReadyButtonClicked()
    {
        PlayerReadyState.Instance.SubmitReadyServerRpc();  // ������ �غ� �Ϸ� ����
        readyButton.SetActive(false);  // ��ư ��Ȱ��ȭ
    }
}
