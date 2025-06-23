using UnityEngine;

public class IntroSceneManager : MonoBehaviour
{
    void Start()
    {
        Invoke("GoToStage1", 2f);
    }

    void GoToStage1()
    {
        GameSceneManager.Instance?.LoadStage1();
    }
}
