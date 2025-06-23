using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class UiManagerTank : MonoBehaviour
{
    public TMP_Text textUserInfo;
    public Slider sliderHealth;

    public int health = 0;
    public int maxHealth = 100;

    [SerializeField] private GameObject panelGameEndMenu; //ESC �޴� �г�

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

        SoundManager.Instance.f_StopAllBGM();  //��� ������� ����
        SoundManager.Instance.f_AutoPlayBGM(); //Tank ������ �ڵ����� ������� ���

        health = maxHealth;
        updateTextUserInfo();
        updateSliderHealth();
        Invoke("checkCountId", 2.0f);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape)) //ESC Ű ������ ���� �޴� �г� ���
        {
            panelGameEndMenu.SetActive(!panelGameEndMenu.activeSelf); //�г� Ȱ��ȭ/��Ȱ��ȭ ���
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

    public void f_OnClick_Resume() //���� �簳 ��ư Ŭ�� �� ȣ��(����Ű�� ESC)
    {
        panelGameEndMenu.SetActive(false); //�޴� �г� ��Ȱ��ȭ
    }

    public void f_OnClick_Quit() //���� ���� ��ư Ŭ�� �� ȣ��
    {
        Application.Quit();
    }
}
