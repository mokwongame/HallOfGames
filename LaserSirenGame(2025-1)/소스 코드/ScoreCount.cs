using TMPro;
using UnityEngine;

public class ScoreCount : MonoBehaviour
{


    [SerializeField] TMP_Text highScore;
    [SerializeField] TMP_Text score;
    void Start()
    {
        highScore.text = $"HighScore : {PlayerPrefs.GetInt("HighScore")}";
        score.text = $"Score : {PlayerPrefs.GetInt("Score")}";
        //HighScore : 00000
    }

}
