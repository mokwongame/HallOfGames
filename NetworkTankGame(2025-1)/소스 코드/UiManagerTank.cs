using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class UiManagerTank : MonoBehaviour
{
    public TMP_Text textUserInfo;
    public Slider sliderHealth;

    public int health = 0;
    public int maxHealth = 100;

    [SerializeField] private GameObject panelGameEndMenu; //ESC 메뉴 패널

    private static UiManagerTank _instance = null;
    public static UiManagerTank Instance
    {
        get
        {
            if (_instance == null)
            {
                Debug.Log("UiManagerTank is null.");
            }
            return _instance;
        }
    }

    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        GameManager.Instance.startNode();
        GameManager.Instance.updateCountId();

        SoundManager.Instance.f_StopAllBGM();  //모든 배경음악 정지
        SoundManager.Instance.f_AutoPlayBGM(); //Tank 씬에서 자동으로 배경음악 재생

        health = maxHealth;
        updateTextUserInfo();
        updateSliderHealth();
        Invoke("checkCountId", 2.0f);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape)) //ESC 키 누르면 종료 메뉴 패널 토글
        {
            panelGameEndMenu.SetActive(!panelGameEndMenu.activeSelf); //패널 활성화/비활성화 토글
        }
    }

    private void Awake()
    {
        if (_instance == null)
        {
            _instance = this;
        }
        else if (_instance != this)
        {
            Debug.Log("UiManagerTank has another instance.");
            Destroy(gameObject);
        }
    }

    public void updateTextUserInfo()
    {
        textUserInfo.text = $"{GameManager.Instance.CountId}:{GameManager.Instance.UserId}={health}";
    }

    public void updateSliderHealth()
    {
        sliderHealth.value = health;
    }

    void checkCountId()
    {
        GameManager.Instance.updateCountId();
        updateTextUserInfo();
    }

    public void f_OnClick_Resume() //게임 재개 버튼 클릭 시 호출(단축키는 ESC)
    {
        panelGameEndMenu.SetActive(false); //메뉴 패널 비활성화
    }

    public void f_OnClick_Quit() //게임 종료 버튼 클릭 시 호출
    {
        Application.Quit();
    }
}
