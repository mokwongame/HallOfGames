using UnityEngine;
using UnityEngine.SceneManagement;

public class GameSceneManager : MonoBehaviour
{
    public static GameSceneManager Instance;

    private void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    public void LoadStartScene()
    {
        SceneManager.LoadScene("StartScene");
    }

    public void LoadIntroScene()
    {
        SceneManager.LoadScene("IntroScene");
    }

    public void LoadStage1()
    {
        SceneManager.LoadScene("Stage1");
    }

    public void LoadStage2()
    {
        SceneManager.LoadScene("Stage2");
    }

    public void LoadStage3()
    {
        SceneManager.LoadScene("Stage3");
    }

    public void LoadClearScene()
    {
        SceneManager.LoadScene("ClearScene");
    }

    public void LoadEndScene()
    {
        SceneManager.LoadScene("EndScene");
    }

    public void QuitGame()
    {
        AudioManager.Instance?.PlayButtonClickSFX();
        Application.Quit();
        Debug.Log("게임 종료");
    }

    // Start 버튼 누르면 실행되는 함수
    public void OnStartButton()
    {
        AudioManager.Instance?.PlayButtonClickSFX();
        LoadIntroScene();
    }


    // 게임 종료 버튼
    public void OnQuitButton()
    {
        AudioManager.Instance?.PlayButtonClickSFX();  
        QuitGame();
    }
}
