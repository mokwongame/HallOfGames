using UnityEngine;
using UnityEngine.SceneManagement;


public class UiManagerInit : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        //각 매니저들의 초기화만 담당하는 Init씬 이므로 "MainMenuScene" 씬으로 바로 전환
        SceneManager.LoadScene("MainMenuScene");
    }
}
