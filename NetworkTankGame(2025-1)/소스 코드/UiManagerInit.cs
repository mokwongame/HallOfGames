using UnityEngine;
using UnityEngine.SceneManagement;


public class UiManagerInit : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        //�� �Ŵ������� �ʱ�ȭ�� ����ϴ� Init�� �̹Ƿ� "MainMenuScene" ������ �ٷ� ��ȯ
        SceneManager.LoadScene("MainMenuScene");
    }
}
