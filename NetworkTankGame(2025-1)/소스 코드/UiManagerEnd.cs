using UnityEngine;

public class UiManagerEnd : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        SoundManager.Instance.f_StopAllBGM(); //모든 배경음악 정지
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void f_OnClickExitButton() //게임 종료 버튼 클릭 시 애플리케이션을 종료하는 메소드
    {
        Application.Quit(); //애플리케이션 종료
    }
}
