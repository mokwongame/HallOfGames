using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class playact : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Invoke("startGame",5.0f);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void startGame(){
        SceneManager.LoadScene("BowlingGame");
    }
}
