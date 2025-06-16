using UnityEngine;

public class MoveBullet : MonoBehaviour
{
    [SerializeField] float speed;

    [SerializeField] LayerMask mask;

    private void Start()
    {
        Destroy(gameObject, 1);
    }

    void Update()
    {

        RaycastHit[] hits = Physics.RaycastAll(transform.position, -transform.forward, speed * Time.deltaTime, mask);
        
        foreach (RaycastHit hit in hits)
        {

            if (hit.transform.gameObject.layer == LayerMask.NameToLayer("Mole"))
            {
                //두더지에게 데미지
                if (hit.transform.gameObject.GetComponent<Mole>() != null)
                {
                    Mole mole = hit.transform.gameObject.GetComponent<Mole>();
                    mole.GetDamage(1);
                }
            }
        }
        transform.Translate(0, 0, -speed * Time.deltaTime);


        //필드나 두더지에게 부디쳤을 경우
        if (hits.Length != 0)
        {
            Destroy(gameObject);
        }
    }


}
