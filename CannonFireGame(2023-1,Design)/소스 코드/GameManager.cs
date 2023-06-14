using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    int score = 0;
    public TMP_Text ScoreText;
    // Start is called before the first frame update
    void Start()
    {
        writeScore();
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            SceneManager.LoadScene("EndScene");
        }
    }

    public int getScore() { return score; }
    public void inScore() { score++; writeScore(); }
    public void outScore() { score--; writeScore(); }
    public void writeScore() { ScoreText.text = score.ToString(); }
}
