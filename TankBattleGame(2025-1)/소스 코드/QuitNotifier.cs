using Unity.Netcode;

// 클라이언트가 종료를 요청했을 때 서버에서 처리하는 클래스
public class QuitNotifier : NetworkBehaviour
{
    // 클라이언트가 종료를 서버에 알리는 ServerRpc 메서드
    // RequireOwnership = false → 호출자가 오브젝트의 소유자가 아니어도 호출 가능
    [ServerRpc(RequireOwnership = false)]
    public void NotifyQuitServerRpc(ServerRpcParams rpcParams = default)
    {
        // 요청을 보낸 클라이언트의 ID를 가져옴
        ulong clientId = rpcParams.Receive.SenderClientId;

        // 해당 클라이언트가 실제 연결되어 있는지 확인
        if (NetworkManager.Singleton.ConnectedClients.TryGetValue(clientId, out var client))
        {
            var playerObj = client.PlayerObject;

            // 플레이어 오브젝트가 존재하면 네트워크에서 제거 (Despawn)
            if (playerObj)
            {
                playerObj.Despawn();  // 네트워크에서 플레이어 제거
            }
        }
    }
}