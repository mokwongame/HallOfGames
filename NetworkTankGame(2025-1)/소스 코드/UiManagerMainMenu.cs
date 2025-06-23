using UnityEngine;

public class UiManagerMainMenu : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        SoundManager.Instance.f_AutoPlayBGM(); //메인 메뉴 씬에서 자동으로 배경음악 재생
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void f_OnClickStartButton() //게임 시작 버튼 클릭 시 "BeginScene" 씬으로 전환하는 메소드
    {
        FlowManager.Instance.f_OpenScene(SceneName.BeginScene); //게임 시작 버튼 클릭 시 "BeginScene" 씬으로 전환
    }

    public void f_OnClickExitButton() //게임 종료 버튼 클릭 시 애플리케이션을 종료하는 메소드
    {
        Application.Quit(); //애플리케이션 종료
    }

}
