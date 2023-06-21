using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameStart : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Invoke("gameStart", 30.0f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void gameStart()
    {
        SceneManager.LoadScene("SuperRunner");
    }
}
