using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.SceneManagement;

public class NewGameAct : MonoBehaviour
{
    public TMP_InputField inputName;
    public AudioSource buttonSound;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    public void startGame()
    {
        // 버튼 사운드 효과 재생
        if (buttonSound != null)
        {
            buttonSound.Play();
        }

        string userName = inputName.text;
        if (userName.Length <= 0) // 이름 입력 안함
        {
            userName = "none";
        }
        // 사용자 설정: PlayerPrefs
        // 저장하기: Set
        // 불러오기: Get
        PlayerPrefs.SetString("userName", userName); // key, value
        SceneManager.LoadScene("Main");
    }
}
