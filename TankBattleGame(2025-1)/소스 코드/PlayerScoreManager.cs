using Unity.Collections;
using Unity.Netcode;

// 플레이어 점수와 사용자 ID를 관리하는 클래스
public class PlayerScoreManager : NetworkBehaviour
{
    // 네트워크 변수로 사용자 ID를 저장 (최대 64바이트 고정 길이 문자열)
    public NetworkVariable<FixedString64Bytes> userId = new();

    // 네트워크에 오브젝트가 스폰될 때 호출
    public override void OnNetworkSpawn()
    {
        // 로컬 소유자일 경우 서버에 사용자 ID를 설정하도록 RPC 호출
        if (IsOwner)
        {
            SetUserIdServerRpc(BeginGameManager.Instance.UserId);
        }

        base.OnNetworkSpawn();
    }

    // 서버 RPC: 특정 클라이언트 소유자의 점수를 서버에 추가 요청
    // RequireOwnership = false 이므로 소유자가 아니어도 호출 가능
    [ServerRpc(RequireOwnership = false)]
    public void AddScoreServerRpc(ulong ownerClientId, int amount)
    {
        // 서버의 InGameManager에 점수 저장 함수 호출
        InGameManager.Instance.CachePlayerScore(ownerClientId, userId.Value.Value, amount);
    }

    // 서버 RPC: 사용자 이름을 서버에 설정하는 함수 (비공개)
    [ServerRpc(RequireOwnership = false)]
    private void SetUserIdServerRpc(string userName)
    {
        // 네트워크 변수에 사용자 ID 저장
        userId.Value = userName;
    }
}
