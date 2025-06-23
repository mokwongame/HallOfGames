using System.Collections;
using Unity.Netcode;
using Unity.Netcode.Transports.UTP;
using UnityEngine;
using UnityEngine.SceneManagement;

// 게임 종료 화면에서 UI를 관리하는 클래스
public class UiManagerEnd : MonoBehaviour
{
    // 시작 시 호출: 종료 테마 음악 재생
    private void Start()
    {
        SoundManager.Instance.PlaySound(SoundType.ThemeEnd, true);  // 반복 재생
    }

    // '메인 메뉴로' 버튼 클릭 시 호출
    public void OnMainMenuBtnClicked()
    {
        SoundManager.Instance.StopPlaySound(SoundType.ThemeEnd);  // 음악 정지
        StartCoroutine(nameof(ReturnToMainMenu));                 // 메인 메뉴로 돌아가는 코루틴 실행
    }

    // '게임 종료' 버튼 클릭 시 호출
    public void OnExitBtnClicked()
    {
        SoundManager.Instance.StopPlaySound(SoundType.ThemeEnd);    // 음악 정지

#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;            // 에디터 실행 중단
#else
        Application.Quit();  // 빌드된 애플리케이션 종료
#endif
    }

    // 네트워크 종료 후 메인 메뉴 씬으로 전환하는 코루틴
    private IEnumerator ReturnToMainMenu()
    {
        // 서버 또는 클라이언트가 실행 중이라면 종료 요청
        if (NetworkManager.Singleton.IsListening)
        {
            NetworkManager.Singleton.Shutdown();  // 네트워크 종료
        }

        // 완전히 종료될 때까지 대기
        yield return new WaitUntil(() => NetworkManager.Singleton.IsListening == false);

        // 메인 메뉴 씬 로드
        SceneManager.LoadScene("BeginScene");
    }
}
