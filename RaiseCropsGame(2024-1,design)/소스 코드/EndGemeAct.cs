using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class EndGameAct : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.C))
        {
            SceneManager.LoadScene("SampleScene");
        }
        else if (Input.GetKeyDown(KeyCode.V))
        {
            Application.Quit();
            print("exit");
        }
    }
}
