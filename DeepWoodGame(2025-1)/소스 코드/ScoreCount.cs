using TMPro;
using UnityEngine;
using UnityEngine.SocialPlatforms.Impl;

public class ScoreCount : MonoBehaviour
{


    [SerializeField] TMP_Text score;
    void Start()
    {
        score.text = $"Score : {PlayerPrefs.GetInt("Score")}";
    }
}
