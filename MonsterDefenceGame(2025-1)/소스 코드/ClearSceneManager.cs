using UnityEngine;
using UnityEngine.SceneManagement;

public class ClearSceneManager : MonoBehaviour
{
    void Start()
    {
        Invoke("GoToEndScene", 17f);
    }

    void GoToEndScene()
    {
        Debug.Log("GoToEndScene 호출됨");
        SceneManager.LoadScene("EndScene");
    }
}
