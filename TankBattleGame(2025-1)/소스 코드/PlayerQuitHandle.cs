using Unity.Netcode;
using UnityEngine;

// 플레이어 종료 시 네트워크에 알림을 보내는 싱글톤 클래스
public class PlayerQuitHandle : MonoBehaviour
{
    // 싱글톤 인스턴스 프로퍼티
    public static PlayerQuitHandle Instance { get; private set; }

    // 오브젝트 생성 시 싱글톤 패턴 적용
    private void Awake()
    {
        // 이미 인스턴스가 존재하고 자신이 아니면 중복 오브젝트 제거
        if (Instance && Instance != this)
        {
            Destroy(gameObject);
            return;
        }

        // 인스턴스 할당 및 씬 전환 시에도 파괴되지 않도록 설정
        Instance = this;
        DontDestroyOnLoad(gameObject);
    }

    // 애플리케이션이 종료될 때 호출되는 콜백 함수
    private void OnApplicationQuit()
    {
        // 네트워크 매니저가 없거나 클라이언트가 연결되어 있지 않으면 리턴
        if (!NetworkManager.Singleton || !NetworkManager.Singleton.IsConnectedClient)
        {
            return;
        }

        // 로컬 플레이어 오브젝트를 얻어옴
        var playerObject = NetworkManager.Singleton.SpawnManager.GetLocalPlayerObject();

        // 플레이어 오브젝트가 있고 QuitNotifier 컴포넌트를 가지고 있으면
        if (playerObject && playerObject.TryGetComponent(out QuitNotifier notifier))
        {
            // 서버에 종료 알림 RPC 호출 (서버에 플레이어 종료 통보)
            notifier.NotifyQuitServerRpc();
        }
    }
}
