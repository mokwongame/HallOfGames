using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameOver : MonoBehaviour
{
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.R)) // �����
        {
            GameScene();
        }
        else if (Input.GetKeyDown(KeyCode.X)) // ����
        {
            GameQuit();
        }
    }

    public void GameScene()
    {
        SceneManager.LoadScene("SuperRunner");
    }

    public void GameQuit()
    {
        Application.Quit();
        Debug.Log("Quit");
    }
}
