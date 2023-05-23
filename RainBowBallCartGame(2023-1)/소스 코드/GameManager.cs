using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public TMP_Text TextScore;
    int Score;
    // Start is called before the first frame update
    void Start()
    {
        Score = 0;
        TextScore.text = Score.ToString();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public int GetScore() { return Score; }

    public void IncScore()
    {
        Score = Score + 2;
        TextScore.text = Score.ToString();
        if (Score >= 25)
        {
            SceneManager.LoadScene("GameEndScene");
        }
    }
    public void DecScore()
    {
        Score = Score - 1;
        TextScore.text = Score.ToString();
    }
}
