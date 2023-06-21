using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public TMP_Text textScore, textHeart;
    int score = 0;
    public int heart = 5;
    void Start()
    {
        showScore();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void incScore()
    {
        score++;
        showScore();
    }
    void showScore()
    {
        textScore.text = score.ToString();
    }

    public void decHeart()
    {
        heart--;
        showHeart();
    }

    void showHeart()
    {
        textHeart.text = heart.ToString();
    }
    public void PlayStart()
    {
        SceneManager.LoadScene("SuperRunner");
    }
}
